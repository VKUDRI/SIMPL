/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "FileListInfo.h"

// -----------------------------------------------------------------------------
FileListInfo::FileListInfo() = default;

// -----------------------------------------------------------------------------
FileListInfo::~FileListInfo() = default;

// -----------------------------------------------------------------------------
void FileListInfo::writeSuperclassJson(QJsonObject& json) const
{
  json["PaddingDigits"] = static_cast<qint32>(PaddingDigits);
  json["Ordering"] = static_cast<qint32>(Ordering);
  json["IncrementIndex"] = static_cast<qint32>(IncrementIndex);
  json["InputPath"] = InputPath;
  json["FilePrefix"] = FilePrefix;
  json["FileSuffix"] = FileSuffix;
  json["FileExtension"] = FileExtension;
}

// -----------------------------------------------------------------------------
bool FileListInfo::readSuperclassJson(QJsonObject& json)
{
  if(json["PaddingDigits"].isDouble() && json["Ordering"].isDouble() && json["IncrementIndex"].isDouble() && json["InputPath"].isString() && json["FilePrefix"].isString() &&
     json["FileSuffix"].isString() && json["FileExtension"].isString())
  {
    PaddingDigits = static_cast<qint32>(json["PaddingDigits"].toInt());
    Ordering = static_cast<quint32>(json["Ordering"].toInt());
    IncrementIndex = static_cast<qint32>(json["IncrementIndex"].toInt());
    InputPath = json["InputPath"].toString();
    FilePrefix = json["FilePrefix"].toString();
    FileSuffix = json["FileSuffix"].toString();
    FileExtension = json["FileExtension"].toString();
    return true;
  }
  return false;
}
