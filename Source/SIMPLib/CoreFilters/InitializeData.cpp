/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the following contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "InitializeData.h"

#include <chrono>
#include <random>
#include <thread>

#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QTime>

#include "SIMPLib/SIMPLibVersion.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

namespace Detail
{
template <typename T>
class UniformDistribution
{
public:
  virtual T generateValue() = 0;
};

template <typename T>
class UniformIntDistribution : public UniformDistribution<T>
{
public:
  UniformIntDistribution(T rangeMin, T rangeMax)
  {
    std::random_device randomDevice;               // Will be used to obtain a seed for the random number engine
    m_Generator = std::mt19937_64(randomDevice()); // Standard mersenne_twister_engine seeded with rd()
    std::mt19937_64::result_type seed = static_cast<std::mt19937_64::result_type>(std::chrono::steady_clock::now().time_since_epoch().count());
    m_Generator.seed(seed);
    m_Distribution = std::uniform_int_distribution<T>(rangeMin, rangeMax);
  }

  T generateValue() override
  {
    return m_Distribution(m_Generator);
  }

private:
  std::uniform_int_distribution<T> m_Distribution;
  std::mt19937_64 m_Generator;
};

template <typename T>
class UniformRealsDistribution : public UniformDistribution<T>
{
public:
  UniformRealsDistribution(T rangeMin, T rangeMax)
  {
    std::random_device randomDevice;               // Will be used to obtain a seed for the random number engine
    m_Generator = std::mt19937_64(randomDevice()); // Standard mersenne_twister_engine seeded with rd()
    std::mt19937_64::result_type seed = static_cast<std::mt19937_64::result_type>(std::chrono::steady_clock::now().time_since_epoch().count());
    m_Generator.seed(seed);
    m_Distribution = std::uniform_real_distribution<T>(rangeMin, rangeMax);
  }

  T generateValue() override
  {
    return m_Distribution(m_Generator);
  }

private:
  std::uniform_real_distribution<T> m_Distribution;
  std::mt19937_64 m_Generator;
};

template <typename T>
class UniformBoolDistribution : public UniformDistribution<T>
{
public:
  UniformBoolDistribution()
  {
    std::random_device randomDevice;               // Will be used to obtain a seed for the random number engine
    m_Generator = std::mt19937_64(randomDevice()); // Standard mersenne_twister_engine seeded with rd()
    std::mt19937_64::result_type seed = static_cast<std::mt19937_64::result_type>(std::chrono::steady_clock::now().time_since_epoch().count());
    m_Generator.seed(seed);
    m_Distribution = std::uniform_int_distribution<int>(0, 1);
  }

  T generateValue() override
  {
    int temp = m_Distribution(m_Generator);
    return (temp != 0);
  }

private:
  std::uniform_int_distribution<int> m_Distribution;
  std::mt19937_64 m_Generator;
};

/**
 * @brief checkInitialization Checks that the chosen initialization value/range is inside
 * the bounds of the array type
 */
template <typename T>
void checkInitialization(InitializeData* filter, IDataArray::Pointer p)
{
  QString arrayName = p->getName();

  if(filter->getInitType() == InitializeData::Manual)
  {
    double input = filter->getInitValue();
    if(input < static_cast<double>(std::numeric_limits<T>().lowest()) || input > static_cast<double>(std::numeric_limits<T>().max()))
    {
      QString ss = QObject::tr("%1: The initialization value could not be converted. The valid range is %2 to %3").arg(arrayName).arg(std::numeric_limits<T>::min()).arg(std::numeric_limits<T>::max());
      filter->setErrorCondition(-4000, ss);
      return;
    }
  }
  else if(filter->getInitType() == InitializeData::RandomWithRange)
  {
    FPRangePair initRange = filter->getInitRange();
    double min = initRange.first;
    double max = initRange.second;
    if(min > max)
    {
      QString ss = arrayName + ": Invalid initialization range.  Minimum value is larger than maximum value.";
      filter->setErrorCondition(-5550, ss);
      return;
    }
    if(min < static_cast<double>(std::numeric_limits<T>().lowest()) || max > static_cast<double>(std::numeric_limits<T>().max()))
    {
      QString ss = QObject::tr("%1: The initialization range can only be from %2 to %3").arg(arrayName).arg(std::numeric_limits<T>::min()).arg(std::numeric_limits<T>::max());
      filter->setErrorCondition(-4001, ss);
      return;
    }
    if(min == max)
    {
      QString ss = arrayName + ": The initialization range must have differing values";
      filter->setErrorCondition(-4002, ss);
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <>
void checkInitialization<bool>(InitializeData* filter, IDataArray::Pointer p)
{
  QString arrayName = p->getName();

  if(filter->getInitType() == InitializeData::RandomWithRange)
  {
    FPRangePair initRange = filter->getInitRange();
    double min = initRange.first;
    double max = initRange.second;
    if(min > max)
    {
      QString ss = arrayName + ": Invalid initialization range.  Minimum value is larger than maximum value.";
      filter->setErrorCondition(-4001, ss);
      return;
    }
    if(min == max)
    {
      QString ss = arrayName + ": The initialization range must have differing values";
      filter->setErrorCondition(-4002, ss);
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool isPointInBounds(int64_t i, int64_t j, int64_t k, const std::array<int64_t, 6>& bounds)
{
  return (i >= bounds[0] && i <= bounds[1] && j >= bounds[2] && j <= bounds[3] && k >= bounds[4] && k <= bounds[5]);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
void initializeArray(IDataArray::Pointer p, const std::array<int64_t, 3>& dims, const std::array<int64_t, 6>& bounds, Detail::UniformDistribution<T>& distribution, T manualValue,
                     InitializeData::InitChoices initType, bool invertData)
{
  std::array<int64_t, 6> searchingBounds = bounds;
  if(invertData)
  {
    searchingBounds = {0, dims[0] - 1, 0, dims[1] - 1, 0, dims[2] - 1};
  }

  for(int64_t k = searchingBounds[4]; k <= searchingBounds[5]; k++)
  {
    for(int64_t j = searchingBounds[2]; j <= searchingBounds[3]; j++)
    {
      for(int64_t i = searchingBounds[0]; i <= searchingBounds[1]; i++)
      {
        if(invertData && isPointInBounds(i, j, k, bounds))
        {
          continue;
        }

        size_t index = (k * dims[0] * dims[1]) + (j * dims[0]) + i;

        if(initType == InitializeData::Manual)
        {
          p->initializeTuple(index, &manualValue);
        }
        else
        {
          T value = distribution.generateValue();
          p->initializeTuple(index, &value);
        }
      }
    }
  }
}
} // namespace Detail

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InitializeData::InitializeData() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InitializeData::~InitializeData() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InitializeData::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    MultiDataArraySelectionFilterParameter::RequirementType req =
        MultiDataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_MDA_SELECTION_FP("Cell Arrays", CellAttributeMatrixPaths, FilterParameter::Category::RequiredArray, InitializeData, req));
  }
  parameters.push_back(SIMPL_NEW_INTEGER_FP("X Min (Column)", XMin, FilterParameter::Category::Parameter, InitializeData));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Y Min (Row)", YMin, FilterParameter::Category::Parameter, InitializeData));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Z Min (Plane)", ZMin, FilterParameter::Category::Parameter, InitializeData));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("X Max (Column)", XMax, FilterParameter::Category::Parameter, InitializeData));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Y Max (Row)", YMax, FilterParameter::Category::Parameter, InitializeData));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Z Max (Plane)", ZMax, FilterParameter::Category::Parameter, InitializeData));

  {
    LinkedChoicesFilterParameter::Pointer parameter = LinkedChoicesFilterParameter::New();
    parameter->setHumanLabel("Initialization Type");
    parameter->setPropertyName("InitType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(InitializeData, this, InitType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(InitializeData, this, InitType));

    parameter->setDefaultValue(Manual);

    std::vector<QString> choices;
    choices.push_back("Manual");
    choices.push_back("Random");
    choices.push_back("Random With Range");
    parameter->setChoices(choices);
    std::vector<QString> linkedProps;
    linkedProps.push_back("InitValue");
    linkedProps.push_back("InitRange");
    parameter->setLinkedProperties(linkedProps);
    parameter->setEditable(false);
    parameter->setCategory(FilterParameter::Category::Parameter);
    parameters.push_back(parameter);
  }
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Initialization Value", InitValue, FilterParameter::Category::Parameter, InitializeData, Manual));
  parameters.push_back(SIMPL_NEW_RANGE_FP("Initialization Range", InitRange, FilterParameter::Category::Parameter, InitializeData, RandomWithRange));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Invert", InvertData, FilterParameter::Category::Parameter, InitializeData));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InitializeData::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InitializeData::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  if(m_CellAttributeMatrixPaths.empty())
  {
    QString ss = "At least one data array must be selected.";
    setErrorCondition(-5550, ss);
    return;
  }

  DataArrayPath attributeMatrixPath(m_CellAttributeMatrixPaths[0].getDataContainerName(), m_CellAttributeMatrixPaths[0].getAttributeMatrixName(), "");
  AttributeMatrix::Pointer am = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, attributeMatrixPath, -301);
  if(getErrorCode() < 0)
  {
    return;
  }

  ImageGeom::Pointer image = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom>(this, attributeMatrixPath.getDataContainerName());
  if(nullptr == image.get())
  {
    return;
  }

  if(getXMax() < getXMin())
  {
    QString ss = QObject::tr("X Max (%1) less than X Min (%2)").arg(getXMax()).arg(getXMin());
    setErrorCondition(-5551, ss);
  }
  if(getYMax() < getYMin())
  {
    QString ss = QObject::tr("Y Max (%1) less than Y Min (%2)").arg(getYMax()).arg(getYMin());
    setErrorCondition(-5552, ss);
  }
  if(getZMax() < getZMin())
  {
    QString ss = QObject::tr("Z Max (%1) less than Z Min (%2)").arg(getZMax()).arg(getZMin());
    setErrorCondition(-5553, ss);
  }
  if(getXMin() < 0)
  {
    QString ss = QObject::tr("X Min (%1) less than 0").arg(getXMin());
    setErrorCondition(-5554, ss);
  }
  if(getYMin() < 0)
  {
    QString ss = QObject::tr("Y Min (%1) less than 0").arg(getYMin());
    setErrorCondition(-5555, ss);
  }
  if(getZMin() < 0)
  {
    QString ss = QObject::tr("Z Min (%1) less than 0").arg(getZMin());
    setErrorCondition(-5556, ss);
  }
  if(getXMax() > (static_cast<int64_t>(image->getXPoints()) - 1))
  {
    QString ss = QObject::tr("The X Max you entered of %1 is greater than your Max X Point of %2").arg(getXMax()).arg(static_cast<int64_t>(image->getXPoints()) - 1);
    setErrorCondition(-5557, ss);
  }
  if(getYMax() > (static_cast<int64_t>(image->getYPoints()) - 1))
  {
    QString ss = QObject::tr("The Y Max you entered of %1 is greater than your Max Y Point of %2").arg(getYMax()).arg(static_cast<int64_t>(image->getYPoints()) - 1);
    setErrorCondition(-5558, ss);
  }
  if(getZMax() > (static_cast<int64_t>(image->getZPoints()) - 1))
  {
    QString ss = QObject::tr("The Z Max you entered of %1) greater than your Max Z Point of %2").arg(getZMax()).arg(static_cast<int64_t>(image->getZPoints()) - 1);
    setErrorCondition(-5559, ss);
  }

  std::vector<QString> voxelArrayNames = DataArrayPath::GetDataArrayNames(m_CellAttributeMatrixPaths);

  for(const QString& name : voxelArrayNames)
  {
    IDataArray::Pointer p = am->getAttributeArray(name);
    if(p == nullptr)
    {
      setErrorCondition(-5560, QString("DataArray \"%1\" doesn't exist").arg(name));
      return;
    }

    QString type = p->getTypeAsString();
    if(type == "int8_t")
    {
      checkInitialization<int8_t>(p);
    }
    else if(type == "int16_t")
    {
      checkInitialization<int16_t>(p);
    }
    else if(type == "int32_t")
    {
      checkInitialization<int32_t>(p);
    }
    else if(type == "int64_t")
    {
      checkInitialization<int64_t>(p);
    }
    else if(type == "uint8_t")
    {
      checkInitialization<uint8_t>(p);
    }
    else if(type == "uint16_t")
    {
      checkInitialization<uint16_t>(p);
    }
    else if(type == "uint32_t")
    {
      checkInitialization<uint32_t>(p);
    }
    else if(type == "uint64_t")
    {
      checkInitialization<uint64_t>(p);
    }
    else if(type == "float")
    {
      checkInitialization<float>(p);
    }
    else if(type == "double")
    {
      checkInitialization<double>(p);
    }
    else if(type == "bool")
    {
      checkInitialization<bool>(p);
    }

    if(getErrorCode() < 0)
    {
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
void InitializeData::checkInitialization(IDataArray::Pointer p)
{
  Detail::checkInitialization<T>(this, p);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InitializeData::execute()
{
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  DataArrayPath attributeMatrixPath(m_CellAttributeMatrixPaths[0].getDataContainerName(), m_CellAttributeMatrixPaths[0].getAttributeMatrixName(), "");
  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(attributeMatrixPath.getDataContainerName());

  SizeVec3Type udims = m->getGeometryAs<ImageGeom>()->getDimensions();

  std::array<int64_t, 3> dims = {
      static_cast<int64_t>(udims[0]),
      static_cast<int64_t>(udims[1]),
      static_cast<int64_t>(udims[2]),
  };

  std::array<int64_t, 6> bounds = {m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax};

  QString attrMatName = attributeMatrixPath.getAttributeMatrixName();
  std::vector<QString> voxelArrayNames = DataArrayPath::GetDataArrayNames(m_CellAttributeMatrixPaths);

  for(const QString& name : voxelArrayNames)
  {
    IDataArray::Pointer p = m->getAttributeMatrix(attrMatName)->getAttributeArray(name);

    QString type = p->getTypeAsString();
    if(type == "int8_t")
    {
      initializeArrayWithInts<int8_t>(p, dims, bounds);
    }
    else if(type == "int16_t")
    {
      initializeArrayWithInts<int16_t>(p, dims, bounds);
    }
    else if(type == "int32_t")
    {
      initializeArrayWithInts<int32_t>(p, dims, bounds);
    }
    else if(type == "int64_t")
    {
      initializeArrayWithInts<int64_t>(p, dims, bounds);
    }
    else if(type == "uint8_t")
    {
      initializeArrayWithInts<uint8_t>(p, dims, bounds);
    }
    else if(type == "uint16_t")
    {
      initializeArrayWithInts<uint16_t>(p, dims, bounds);
    }
    else if(type == "uint32_t")
    {
      initializeArrayWithInts<uint32_t>(p, dims, bounds);
    }
    else if(type == "uint64_t")
    {
      initializeArrayWithInts<uint64_t>(p, dims, bounds);
    }
    else if(type == "float")
    {
      initializeArrayWithReals<float>(p, dims, bounds);
    }
    else if(type == "double")
    {
      initializeArrayWithReals<double>(p, dims, bounds);
    }
    else if(type == "bool")
    {
      initializeArrayWithBools(p, dims, bounds);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Delay the execution to avoid the exact same seedings for each array
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
void InitializeData::initializeArrayWithInts(IDataArray::Pointer p, std::array<int64_t, 3>& dims, std::array<int64_t, 6>& bounds)
{
  std::pair<T, T> range = getRange<T>();
  Detail::UniformIntDistribution<T> distribution(range.first, range.second);
  T manualValue = static_cast<T>(m_InitValue);
  Detail::initializeArray(p, dims, bounds, distribution, manualValue, static_cast<InitializeData::InitChoices>(m_InitType), m_InvertData);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
void InitializeData::initializeArrayWithReals(IDataArray::Pointer p, std::array<int64_t, 3>& dims, std::array<int64_t, 6>& bounds)
{
  std::pair<T, T> range = getRange<T>();
  Detail::UniformRealsDistribution<T> distribution(range.first, range.second);
  T manualValue = static_cast<T>(m_InitValue);
  Detail::initializeArray(p, dims, bounds, distribution, manualValue, static_cast<InitializeData::InitChoices>(m_InitType), m_InvertData);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InitializeData::initializeArrayWithBools(IDataArray::Pointer p, std::array<int64_t, 3>& dims, std::array<int64_t, 6>& bounds)
{
  Detail::UniformBoolDistribution<bool> distribution;
  bool manualValue = (m_InitValue != 0);
  Detail::initializeArray(p, dims, bounds, distribution, manualValue, static_cast<InitializeData::InitChoices>(m_InitType), m_InvertData);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
std::pair<T, T> InitializeData::getRange()
{
  if(m_InitType == RandomWithRange)
  {
    return m_InitRange;
  }
  return std::make_pair(std::numeric_limits<T>().min(), std::numeric_limits<T>().max());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer InitializeData::newFilterInstance(bool copyFilterParameters) const
{
  InitializeData::Pointer filter = InitializeData::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InitializeData::getCompiledLibraryName() const
{
  return Core::CoreBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InitializeData::getBrandingString() const
{
  return "SIMPLib Core Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InitializeData::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << SIMPLib::Version::Major() << "." << SIMPLib::Version::Minor() << "." << SIMPLib::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InitializeData::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid InitializeData::getUuid() const
{
  return QUuid("{dfab9921-fea3-521c-99ba-48db98e43ff8}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InitializeData::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::ConversionFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString InitializeData::getHumanLabel() const
{
  return "Initialize Data";
}

// -----------------------------------------------------------------------------
InitializeData::Pointer InitializeData::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<InitializeData> InitializeData::New()
{
  struct make_shared_enabler : public InitializeData
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString InitializeData::getNameOfClass() const
{
  return QString("InitializeData");
}

// -----------------------------------------------------------------------------
QString InitializeData::ClassName()
{
  return QString("InitializeData");
}

// -----------------------------------------------------------------------------
void InitializeData::setCellAttributeMatrixPaths(const std::vector<DataArrayPath>& value)
{
  m_CellAttributeMatrixPaths = value;
}

// -----------------------------------------------------------------------------
std::vector<DataArrayPath> InitializeData::getCellAttributeMatrixPaths() const
{
  return m_CellAttributeMatrixPaths;
}

// -----------------------------------------------------------------------------
void InitializeData::setXMin(int value)
{
  m_XMin = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getXMin() const
{
  return m_XMin;
}

// -----------------------------------------------------------------------------
void InitializeData::setYMin(int value)
{
  m_YMin = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getYMin() const
{
  return m_YMin;
}

// -----------------------------------------------------------------------------
void InitializeData::setZMin(int value)
{
  m_ZMin = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getZMin() const
{
  return m_ZMin;
}

// -----------------------------------------------------------------------------
void InitializeData::setXMax(int value)
{
  m_XMax = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getXMax() const
{
  return m_XMax;
}

// -----------------------------------------------------------------------------
void InitializeData::setYMax(int value)
{
  m_YMax = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getYMax() const
{
  return m_YMax;
}

// -----------------------------------------------------------------------------
void InitializeData::setZMax(int value)
{
  m_ZMax = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getZMax() const
{
  return m_ZMax;
}

// -----------------------------------------------------------------------------
void InitializeData::setInitType(int value)
{
  m_InitType = value;
}

// -----------------------------------------------------------------------------
int InitializeData::getInitType() const
{
  return m_InitType;
}

// -----------------------------------------------------------------------------
void InitializeData::setRandom(bool value)
{
  m_Random = value;
}

// -----------------------------------------------------------------------------
bool InitializeData::getRandom() const
{
  return m_Random;
}

// -----------------------------------------------------------------------------
void InitializeData::setInitValue(double value)
{
  m_InitValue = value;
}

// -----------------------------------------------------------------------------
double InitializeData::getInitValue() const
{
  return m_InitValue;
}

// -----------------------------------------------------------------------------
void InitializeData::setInitRange(const FPRangePair& value)
{
  m_InitRange = value;
}

// -----------------------------------------------------------------------------
FPRangePair InitializeData::getInitRange() const
{
  return m_InitRange;
}

// -----------------------------------------------------------------------------
void InitializeData::setInvertData(bool value)
{
  m_InvertData = value;
}

// -----------------------------------------------------------------------------
bool InitializeData::getInvertData() const
{
  return m_InvertData;
}
