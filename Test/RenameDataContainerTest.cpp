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
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "SIMPLib/Common/FilterFactory.hpp"
#include "SIMPLib/Common/FilterManager.h"
#include "SIMPLib/Common/FilterPipeline.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Utilities/QMetaObjectUtilities.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLib/CoreFilters/RenameDataContainer.h"

#include "SIMPLTestFileLocations.h"

class RenameDataContainerTest
{
public:
  RenameDataContainerTest()
  {
  }
  virtual ~RenameDataContainerTest()
  {
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestEmptyDataContainerArray()
  {
    RenameDataContainer::Pointer filter = RenameDataContainer::New();
    DREAM3D_ASSERT(nullptr != filter);

    DataContainerArray::Pointer dca = DataContainerArray::New();

    filter->setDataContainerArray(dca);

    QVariant value;

    QString dataContainerName = "DataContainer";
    value.setValue(dataContainerName);
    filter->setProperty("SelectedDataContainerName", value);

    QString newDataContainerName = "DataContainer_";
    value.setValue(dataContainerName);
    filter->setProperty("NewDataContainerName", value);

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -999);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestNullDataContainer()
  {
    RenameDataContainer::Pointer filter = RenameDataContainer::New();
    DREAM3D_ASSERT(nullptr != filter);

    DataContainerArray::Pointer dca = DataContainerArray::New();

    DataContainer::Pointer dc = DataContainer::New();
    dc->setName("DataContainer");
    dca->addDataContainer(dc);

    filter->setDataContainerArray(dca);

    QVariant value;

    QString dataContainerName = "";
    value.setValue(dataContainerName);
    filter->setProperty("SelectedDataContainerName", value);

    QString newDataContainerName = "DataContainer_";
    value.setValue(dataContainerName);
    filter->setProperty("NewDataContainerName", value);

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -11001);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestValidDataContainer()
  {
    RenameDataContainer::Pointer filter = RenameDataContainer::New();
    DREAM3D_ASSERT(nullptr != filter);

    DataContainerArray::Pointer dca = DataContainerArray::New();

    DataContainer::Pointer dc = DataContainer::New();
    dc->setName("DataContainer");
    dca->addDataContainer(dc);

    filter->setDataContainerArray(dca);

    QVariant value;

    QString dataContainerName = "DataContainer";
    value.setValue(dataContainerName);
    filter->setProperty("SelectedDataContainerName", value);

    QString newDataContainerName = "Container_2";
    value.setValue(newDataContainerName);
    filter->setProperty("NewDataContainerName", value);

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), 0);

    DREAM3D_REQUIRE_EQUAL(dc->getName(), newDataContainerName);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestSetEmptyName()
  {
    RenameDataContainer::Pointer filter = RenameDataContainer::New();
    DREAM3D_ASSERT(nullptr != filter);

    DataContainerArray::Pointer dca = DataContainerArray::New();

    DataContainer::Pointer dc = DataContainer::New();
    dc->setName("DataContainer");
    dca->addDataContainer(dc);

    filter->setDataContainerArray(dca);

    QVariant value;

    QString dataContainerName = "DataContainer";
    value.setValue(dataContainerName);
    filter->setProperty("SelectedDataContainerName", value);

    QString newDataContainerName = "";
    value.setValue(dataContainerName);
    filter->setProperty("NewDataContainerName", value);

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -11006);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestSetNullName()
  {
    RenameDataContainer::Pointer filter = RenameDataContainer::New();
    DREAM3D_ASSERT(nullptr != filter);

    DataContainerArray::Pointer dca = DataContainerArray::New();

    DataContainer::Pointer dc = DataContainer::New();
    dc->setName("DataContainer");
    dca->addDataContainer(dc);

    filter->setDataContainerArray(dca);

    QVariant value;

    QString dataContainerName = "DataContainer";
    value.setValue(dataContainerName);
    filter->setProperty("SelectedDataContainerName", value);

    QString newDataContainerName = "";
    value.setValue(dataContainerName);

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -11001);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### RenameDataContainerTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestEmptyDataContainerArray());
    DREAM3D_REGISTER_TEST(TestNullDataContainer());
    DREAM3D_REGISTER_TEST(TestValidDataContainer());
    DREAM3D_REGISTER_TEST(TestSetEmptyName());
    DREAM3D_REGISTER_TEST(TestSetNullName());
  }

private:

  RenameDataContainerTest(const RenameDataContainerTest&); // Copy Constructor Not Implemented
  void operator=(const RenameDataContainerTest&);     // Operator '=' Not Implemented
};