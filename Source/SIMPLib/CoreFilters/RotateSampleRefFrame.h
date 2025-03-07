/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <memory>
#include <mutex>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/DataContainers/AttributeMatrix.h"
#include "SIMPLib/FilterParameters/DynamicTableData.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

/**
 * @brief The RotateSampleRefFrame class. See [Filter documentation](@ref rotatesamplerefframe) for details.
 */
class SIMPLib_EXPORT RotateSampleRefFrame : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(RotateSampleRefFrame SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(RotateSampleRefFrame)
  PYB11_FILTER_NEW_MACRO(RotateSampleRefFrame)
  PYB11_ENUMERATION(RotationRepresentation)
  PYB11_PROPERTY(DataArrayPath CellAttributeMatrixPath READ getCellAttributeMatrixPath WRITE setCellAttributeMatrixPath)
  PYB11_PROPERTY(FloatVec3Type RotationAxis READ getRotationAxis WRITE setRotationAxis)
  PYB11_PROPERTY(float RotationAngle READ getRotationAngle WRITE setRotationAngle)
  PYB11_PROPERTY(bool SliceBySlice READ getSliceBySlice WRITE setSliceBySlice)
  PYB11_PROPERTY(DynamicTableData RotationTable READ getRotationTable WRITE setRotationTable)
  PYB11_PROPERTY(int RotationRepresentationChoice READ getRotationRepresentationChoice WRITE setRotationRepresentationChoice)
  PYB11_METHOD(RotationRepresentation getRotationRepresentation)
  PYB11_METHOD(void setRotationRepresentation ARGS value)
  PYB11_METHOD(bool isRotationRepresentationValid ARGS value)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = RotateSampleRefFrame;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;

  /**
   * @brief Returns a NullPointer wrapped by a shared_ptr<>
   * @return
   */
  static Pointer NullPointer();

  /**
   * @brief Creates a new object wrapped in a shared_ptr<>
   * @return
   */
  static Pointer New();

  /**
   * @brief Returns the name of the class for RotateSampleRefFrame
   */
  QString getNameOfClass() const override;

  /**
   * @brief Returns the name of the class for RotateSampleRefFrame
   */
  static QString ClassName();

  ~RotateSampleRefFrame() override;

  enum class RotationRepresentation : int
  {
    AxisAngle = 0,
    RotationMatrix = 1
  };

  /**
   * @brief Returns the current rotation representation in enum form.
   * @return
   */
  RotationRepresentation getRotationRepresentation() const;

  /**
   * @brief Sets the rotation representation value to the given enum value
   * @param value
   */
  void setRotationRepresentation(RotationRepresentation value);

  /**
   * @brief Returns true if the selected index for the rotation representation choice widget is a valid enum value.
   * @param value
   * @return
   */
  bool isRotationRepresentationValid(int value) const;

  /**
   * @brief Setter property for CellAttributeMatrixPath
   */
  void setCellAttributeMatrixPath(const DataArrayPath& value);

  /**
   * @brief Getter property for CellAttributeMatrixPath
   * @return Value of CellAttributeMatrixPath
   */
  DataArrayPath getCellAttributeMatrixPath() const;

  Q_PROPERTY(DataArrayPath CellAttributeMatrixPath READ getCellAttributeMatrixPath WRITE setCellAttributeMatrixPath)

  /**
   * @brief Setter property for RotationAxis
   */
  void setRotationAxis(const FloatVec3Type& value);

  /**
   * @brief Getter property for RotationAxis
   * @return Value of RotationAxis
   */
  FloatVec3Type getRotationAxis() const;

  Q_PROPERTY(FloatVec3Type RotationAxis READ getRotationAxis WRITE setRotationAxis)

  /**
   * @brief Setter property for RotationAngle
   */
  void setRotationAngle(float value);

  /**
   * @brief Getter property for RotationAngle
   * @return Value of RotationAngle
   */
  float getRotationAngle() const;

  Q_PROPERTY(float RotationAngle READ getRotationAngle WRITE setRotationAngle)

  // This is getting exposed because other filters that are calling this filter needs to set this value
  /**
   * @brief Setter property for SliceBySlice
   */
  void setSliceBySlice(bool value);

  /**
   * @brief Getter property for SliceBySlice
   * @return Value of SliceBySlice
   */
  bool getSliceBySlice() const;

  Q_PROPERTY(bool SliceBySlice READ getSliceBySlice WRITE setSliceBySlice)

  /**
   * @brief Setter property for RotationTable
   * @param value
   */
  void setRotationTable(const DynamicTableData& value);

  /**
   * @brief Setter property for RotationRepresentationChoice
   * @param value
   */
  void setRotationRepresentationChoice(int value);

  /**
   * @brief Getter property for RotationRepresentationChoice
   * @return
   */
  int getRotationRepresentationChoice() const;

  Q_PROPERTY(int RotationRepresentationChoice READ getRotationRepresentationChoice WRITE setRotationRepresentationChoice)

  /**
   * @brief Getter property for RotationTable
   * @return
   */
  DynamicTableData getRotationTable() const;

  Q_PROPERTY(DynamicTableData RotationTable READ getRotationTable WRITE setRotationTable)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief sendThreadSafeProgressMessage
   * @param counter
   */
  void sendThreadSafeProgressMessage(int64_t counter);

protected:
  RotateSampleRefFrame();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

public:
  RotateSampleRefFrame(const RotateSampleRefFrame&) = delete;            // Copy Constructor Not Implemented
  RotateSampleRefFrame(RotateSampleRefFrame&&) = delete;                 // Move Constructor Not Implemented
  RotateSampleRefFrame& operator=(const RotateSampleRefFrame&) = delete; // Copy Assignment Not Implemented
  RotateSampleRefFrame& operator=(RotateSampleRefFrame&&) = delete;      // Move Assignment Not Implemented

private:
  struct Impl;
  std::unique_ptr<Impl> p_Impl;

  DataArrayPath m_CellAttributeMatrixPath = {SIMPL::Defaults::ImageDataContainerName, SIMPL::Defaults::CellAttributeMatrixName, ""};
  FloatVec3Type m_RotationAxis = {0.0f, 0.0f, 1.0f};
  float m_RotationAngle = 0.0f;
  bool m_SliceBySlice = false;
  DynamicTableData m_RotationTable;
  int m_RotationRepresentationChoice = 0;

  AttributeMatrix::Pointer m_SourceAttributeMatrix;

  // Threadsafe Progress Message
  mutable std::mutex m_ProgressMessage_Mutex;
  size_t m_InstanceIndex = {0};
  int64_t m_TotalElements = {};

  /**
   * @brief This is an alternate version of the execute that attempted to parallelize over each DataArray. Turns out this was
   * slower then just running it in serial. This is here in case anyone wants to revist this.
   */
  void execute_alt();
};
