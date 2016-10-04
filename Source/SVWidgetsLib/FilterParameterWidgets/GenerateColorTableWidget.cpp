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

#include "GenerateColorTableWidget.h"

#include "SIMPLib/FilterParameters/GenerateColorTableFilterParameter.h"

#include "SVWidgetsLib/Dialogs/ColorPresetsDialog.h"

// Include the MOC generated file for this class
#include "moc_GenerateColorTableWidget.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateColorTableWidget::GenerateColorTableWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent),
  m_DidCausePreflight(false)
{
  m_FilterParameter = dynamic_cast<GenerateColorTableFilterParameter*>(parameter);
  Q_ASSERT_X(m_FilterParameter != nullptr, "NULL Pointer", "GenerateColorTableWidget can ONLY be used with a GenerateColorTableFilterParameter object");
  m_Filter = dynamic_cast<GenerateColorTable*>(filter);
  Q_ASSERT_X(getFilter() != nullptr, "NULL Pointer", "GenerateColorTableWidget can ONLY be used with a GenerateColorTable object");

  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateColorTableWidget::GenerateColorTableWidget(QWidget* parent)
: FilterParameterWidget(nullptr, nullptr, parent),
  m_Filter(nullptr),
  m_FilterParameter(nullptr),
  m_DidCausePreflight(false)
{
  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateColorTableWidget::~GenerateColorTableWidget()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::setFilter(AbstractFilter* value)
{
  m_Filter = dynamic_cast<GenerateColorTable*>(value);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter* GenerateColorTableWidget::getFilter() const
{
  return m_Filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::setFilterParameter(FilterParameter* value)
{
  m_FilterParameter = dynamic_cast<GenerateColorTableFilterParameter*>(value);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterParameter* GenerateColorTableWidget::getFilterParameter() const
{
  return m_FilterParameter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::setupGui()
{

  connect(getFilter(), SIGNAL(preflightAboutToExecute()), this, SLOT(beforePreflight()));

  connect(getFilter(), SIGNAL(preflightExecuted()), this, SLOT(afterPreflight()));

  connect(getFilter(), SIGNAL(updateFilterParameters(AbstractFilter*)), this, SLOT(filterNeedsInputParameters(AbstractFilter*)));

  if(getFilterParameter() != nullptr)
  {
    choosePresetBtn->setText(m_FilterParameter->getHumanLabel());
  }

  if(getFilter() != nullptr)
  {

  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::on_choosePresetBtn_pressed()
{
  ColorPresetsDialog dialog(this);
  dialog.exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
//  // qDebug() << "GenerateColorTableWidget::filterNeedsInputParameters()";
//  m_Filter->setInputFile(filePath->text());
//  updateProxyFromModel(); // Will update m_DcaProxy with the latest selections from the Model
//  m_Filter->setInputFileDataContainerArrayProxy(m_DcaProxy);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::beforePreflight()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateColorTableWidget::afterPreflight()
{

}
