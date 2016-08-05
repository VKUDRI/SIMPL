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
#include "DynamicTableExample.h"

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DynamicTableFilterParameter.h"



// Include the MOC generated file for this class
#include "moc_DynamicTableExample.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DynamicTableExample::DynamicTableExample() :
  AbstractFilter()
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DynamicTableExample::~DynamicTableExample()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DynamicTableExample::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  /* Place all your option initialization code here */

  // Table 1 - Fixed rows and columns, no default data passed in
  {
    DynamicTableData defaultTableData;
    // We are using the expanded way to create the filter parameter, because the getter and setter variables are different
    parameters.push_back(DynamicTableFilterParameter::New("Dynamic Table 1", "DynamicData1", defaultTableData, FilterParameter::Parameter,
                                                          SIMPL_BIND_SETTER(DynamicTableExample, this, DynamicData2),
                                                          SIMPL_BIND_GETTER(DynamicTableExample, this, DynamicData1), false, false, 0));
  }

  // Table 2 - Fixed rows and columns, default data passed in
  {
    DynamicTableData defaultTableData;
    QStringList rHeaders, cHeaders;
    rHeaders << "Passed RowName 1" << "Passed RowName 2";
    cHeaders << "Passed ColName 1" << "Passed ColName 2";
    std::vector<std::vector<double> > defaultTable(4, std::vector<double>(1, 3.87));
    defaultTableData.setColHeaders(cHeaders);
    defaultTableData.setRowHeaders(rHeaders);
    defaultTableData.setTableData(defaultTable);
    parameters.push_back(DynamicTableFilterParameter::New("Dynamic Table 2", "DynamicData2", defaultTableData, FilterParameter::Parameter,
                                                          SIMPL_BIND_SETTER(DynamicTableExample, this, DynamicData2),
                                                          SIMPL_BIND_GETTER(DynamicTableExample, this, DynamicData1), false, false, 0));
  }

  // Table 3 - Dynamic rows and fixed columns, default data passed in
  {
    DynamicTableData defaultTableData;
    QStringList rHeaders, cHeaders;
    rHeaders << "Passed RowName 1" << "Passed RowName 2";
    cHeaders << "Passed ColName 1" << "Passed ColName 2";
    std::vector<std::vector<double> > defaultTable(3, std::vector<double>(5, 1.34));
    defaultTableData.setColHeaders(cHeaders);
    defaultTableData.setRowHeaders(rHeaders);
    defaultTableData.setTableData(defaultTable);
    parameters.push_back(DynamicTableFilterParameter::New("Dynamic Table 3", "DynamicData3", defaultTableData, FilterParameter::Parameter,
                                                          SIMPL_BIND_SETTER(DynamicTableExample, this, DynamicData2),
                                                          SIMPL_BIND_GETTER(DynamicTableExample, this, DynamicData1), true, false, 0));
  }

  // Table 4 - Fixed rows and dynamic columns, default data passed in
  {
    DynamicTableData defaultTableData;
    QStringList rHeaders, cHeaders;
    rHeaders << "Passed RowName 1" << "Passed RowName 2";
    cHeaders << "Passed ColName 1" << "Passed ColName 2";
    std::vector<std::vector<double> > defaultTable(2, std::vector<double>(2, 3));
    defaultTableData.setColHeaders(cHeaders);
    defaultTableData.setRowHeaders(rHeaders);
    defaultTableData.setTableData(defaultTable);
    parameters.push_back(DynamicTableFilterParameter::New("Dynamic Table 4", "DynamicData4", defaultTableData, FilterParameter::Parameter,
                                                          SIMPL_BIND_SETTER(DynamicTableExample, this, DynamicData2),
                                                          SIMPL_BIND_GETTER(DynamicTableExample, this, DynamicData1), false, true, 0));
  }

  // Table 5 - Dynamic rows and dynamic columns, default data passed in
  {
    DynamicTableData defaultTableData;
    QStringList rHeaders, cHeaders;
    rHeaders << "Passed RowName 1" << "Passed RowName 2";
    cHeaders << "Passed ColName 1" << "Passed ColName 2";
    std::vector<std::vector<double> > defaultTable(2, std::vector<double>(2, 3));
    defaultTableData.setColHeaders(cHeaders);
    defaultTableData.setRowHeaders(rHeaders);
    defaultTableData.setTableData(defaultTable);
    parameters.push_back(DynamicTableFilterParameter::New("Dynamic Table 5", "DynamicData5", defaultTableData, FilterParameter::Parameter,
                                                          SIMPL_BIND_SETTER(DynamicTableExample, this, DynamicData2),
                                                          SIMPL_BIND_GETTER(DynamicTableExample, this, DynamicData1), true, true, 0));
  }




  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DynamicTableExample::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{

  reader->openFilterGroup(this, index);
  setDynamicData1(reader->readDynamicTableData("DynamicData", getDynamicData1()));
  setDynamicData2(reader->readDynamicTableData("DynamicData", getDynamicData2()));
  setDynamicData3(reader->readDynamicTableData("DynamicData", getDynamicData3()));
  setDynamicData4(reader->readDynamicTableData("DynamicData", getDynamicData4()));
  setDynamicData5(reader->readDynamicTableData("DynamicData", getDynamicData5()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DynamicTableExample::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DynamicTableExample::dataCheck()
{
  // This filter does nothing during the dataCheck!
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DynamicTableExample::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DynamicTableExample::execute()
{
  // This filter does nothing during execute!

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer DynamicTableExample::newFilterInstance(bool copyFilterParameters)
{
  DynamicTableExample::Pointer filter = DynamicTableExample::New();
  if (true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DynamicTableExample::getCompiledLibraryName()
{
  return Test::TestBaseName;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DynamicTableExample::getGroupName()
{
  return SIMPL::FilterGroups::TestFilters;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DynamicTableExample::getSubGroupName()
{
  return "Misc";
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DynamicTableExample::getHumanLabel()
{
  return "Dynamic Table Example";
}

