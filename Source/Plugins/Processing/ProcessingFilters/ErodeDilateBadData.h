/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
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
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
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
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef ErodeDilateBadData_H_
#define ErodeDilateBadData_H_

#include <vector>
#include <QtCore/QString>


#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/DataContainers/VolumeDataContainer.h"
#include "DREAM3DLib/OrientationOps/OrientationOps.h"
#include "DREAM3DLib/DataArrays/NeighborList.hpp"

#include "Processing/ProcessingConstants.h"

/**
 * @class ErodeDilateBadData ErodeDilateBadData.h DREAM3DLib/ReconstructionFilters/ErodeDilateBadData.h
 * @brief
 * @author
 * @date Nov 19, 2011
 * @version 1.0
 */
class ErodeDilateBadData : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    DREAM3D_SHARED_POINTERS(ErodeDilateBadData)
    DREAM3D_STATIC_NEW_MACRO(ErodeDilateBadData)
    DREAM3D_TYPE_MACRO_SUPER(ErodeDilateBadData, AbstractFilter)

    virtual ~ErodeDilateBadData();
    DREAM3D_INSTANCE_STRING_PROPERTY(DataContainerName)
    DREAM3D_INSTANCE_STRING_PROPERTY(CellAttributeMatrixName)

    DREAM3D_FILTER_PARAMETER(unsigned int, Direction)
    Q_PROPERTY(unsigned int Direction READ getDirection WRITE setDirection)
    DREAM3D_FILTER_PARAMETER(int, NumIterations)
    Q_PROPERTY(int NumIterations READ getNumIterations WRITE setNumIterations)
    DREAM3D_FILTER_PARAMETER(bool, XDirOn)
    Q_PROPERTY(bool XDirOn READ getXDirOn WRITE setXDirOn)
    DREAM3D_FILTER_PARAMETER(bool, YDirOn)
    Q_PROPERTY(bool YDirOn READ getYDirOn WRITE setYDirOn)
    DREAM3D_FILTER_PARAMETER(bool, ZDirOn)
    Q_PROPERTY(bool ZDirOn READ getZDirOn WRITE setZDirOn)

    virtual const QString getCompiledLibraryName() { return Processing::ProcessingBaseName; }
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);
    virtual const QString getGroupName() { return DREAM3D::FilterGroups::ProcessingFilters; }
    virtual const QString getSubGroupName()  { return DREAM3D::FilterSubGroups::CleanupFilters; }
    virtual const QString getHumanLabel() { return "Erode/Dilate Bad Data"; }

    virtual void setupFilterParameters();
    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    virtual void execute();
    virtual void preflight();

  signals:
    void updateFilterParameters(AbstractFilter* filter);
    void parametersChanged();
    void preflightAboutToExecute();
    void preflightExecuted();

  protected:
    ErodeDilateBadData();

  private:
    int32_t* m_Neighbors;
    DEFINE_PTR_WEAKPTR_DATAARRAY(int32_t, FeatureIds)

    QVector<QVector<int> > voxellists;
    QVector<int> nuclei;

    void dataCheck();

    ErodeDilateBadData(const ErodeDilateBadData&); // Copy Constructor Not Implemented
    void operator=(const ErodeDilateBadData&); // Operator '=' Not Implemented
};

#endif /* ErodeDilateBadData_H_ */





