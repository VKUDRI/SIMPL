/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
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

#include "AngleFileLoader.h"

#include <stdio.h>
#include <string.h>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QByteArray>

#include "OrientationLib/OrientationMath/OrientationMath.h"
#include "DREAM3DLib/Math/QuaternionMath.hpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AngleFileLoader::AngleFileLoader():

  m_ErrorMessage(""),
  m_ErrorCode(0),
  m_InputFile(""),
  m_FileAnglesInDegrees(false),
  m_OutputAnglesInDegrees(false),
  m_AngleRepresentation(AngleFileLoader::EulerAngles),
  m_IgnoreMultipleDelimiters(true)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AngleFileLoader::~AngleFileLoader()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatArrayType::Pointer AngleFileLoader::loadData()
{
  FloatArrayType::Pointer angles = FloatArrayType::NullPointer();

  // Make sure the input file variable is not empty
  if (m_InputFile.size() == 0)
  {
    setErrorMessage("Input File Path is empty");
    setErrorCode(-1);
    return angles;
  }

  QFileInfo fi(getInputFile());

  // Make sure the file exists on disk
  if (fi.exists() == false)
  {
    setErrorMessage("Input File does not exist at path");
    setErrorCode(-2);
    return angles;
  }

  // Make sure we have a valid angle representation
  if(m_AngleRepresentation != EulerAngles
      && m_AngleRepresentation != QuaternionAngles
      && m_AngleRepresentation != RodriguezAngles)
  {
    setErrorMessage("The Angle representation was not set to anything know to this code");
    setErrorCode(-3);
    return angles;
  }


  // The format of the file is quite simple. The first line of the file lists the total
  // number of angles in the file. This is followed by a single angle on each line

  int numOrients = 0;
  QByteArray buf;

  // Open the file and read the first line
  QFile reader(getInputFile());
  if (!reader.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString msg = QObject::tr("Angle file could not be opened: %1").arg(getInputFile());
    setErrorCode(-100);
    setErrorMessage(msg);
    return angles;
  }

  bool ok = false;
  buf = reader.readLine();
  buf = buf.trimmed();
  numOrients = buf.toInt(&ok, 10);

  // Allocate enough for the angles
  QVector<size_t> dims(1, 5);
  angles = FloatArrayType::CreateArray(numOrients, dims, "EulerAngles_From_File");

  QList<QByteArray> tokens;

  for(int i = 0; i < numOrients; i++)
  {
    buf = reader.readLine();

    buf = buf.trimmed();

    // Remove multiple Delimiters if wanted by the user.
    if (m_IgnoreMultipleDelimiters == true)
    {
      buf = buf.simplified();
    }
    tokens = buf.split( *(getDelimiter().toLatin1().data()));

    FOrientArrayType euler(3);
    if (m_AngleRepresentation == EulerAngles)
    {
      euler[0] = tokens[0].toFloat(&ok);
      euler[1] = tokens[1].toFloat(&ok);
      euler[2] = tokens[2].toFloat(&ok);
    }
    else if (m_AngleRepresentation == QuaternionAngles)
    {
      FOrientArrayType quat(4);
      quat[0] = tokens[0].toFloat(&ok);
      quat[1] = tokens[1].toFloat(&ok);
      quat[2] = tokens[2].toFloat(&ok);
      quat[3] = tokens[3].toFloat(&ok);
      FOrientTransformsType::qu2eu(quat, euler);
    }
    else if (m_AngleRepresentation == RodriguezAngles)
    {
      FOrientArrayType rod(4, 0.0);
      rod[0] = tokens[0].toFloat(&ok);
      rod[1] = tokens[1].toFloat(&ok);
      rod[2] = tokens[2].toFloat(&ok);
      FOrientTransformsType::ro2eu(rod, euler);
    }

    // Values in File are in Radians and the user wants them in Degrees
    if (m_FileAnglesInDegrees == false && m_OutputAnglesInDegrees == true)
    {
      euler[0] = euler[0] * DREAM3D::Constants::k_RadToDeg;
      euler[1] = euler[1] * DREAM3D::Constants::k_RadToDeg;
      euler[2] = euler[2] * DREAM3D::Constants::k_RadToDeg;
    }
    // Values are in Degrees but user wants them in Radians
    else if (m_FileAnglesInDegrees == true && m_OutputAnglesInDegrees == false)
    {
      euler[0] = euler[0] * DREAM3D::Constants::k_DegToRad;
      euler[1] = euler[1] * DREAM3D::Constants::k_DegToRad;
      euler[2] = euler[2] * DREAM3D::Constants::k_DegToRad;
    }

    float weight = tokens[3].toFloat(&ok);
    float sigma = tokens[4].toFloat(&ok);

    // Store the values into our array
    angles->setComponent(i, 0, euler[0]);
    angles->setComponent(i, 1, euler[1]);
    angles->setComponent(i, 2, euler[2]);
    angles->setComponent(i, 3, weight);
    angles->setComponent(i, 4, sigma);
    //   qDebug() << "reading line: " << i ;
  }



  return angles;
}
