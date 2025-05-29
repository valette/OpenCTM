//-----------------------------------------------------------------------------
// Product:     OpenCTM tools
// File:        vtk.h
// Description: Interface for the vtk file format importer/exporter.
//-----------------------------------------------------------------------------

#ifndef __VTK_H_
#define __VTK_H_

#include <stdlib.h>
#include <iostream>
#include "mesh.h"
#include "convoptions.h"

#ifdef VTKINCLUDED

#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkXMLPolyDataReader.h>

void Import_PolyData(vtkPolyData *mesh, Mesh * aMesh) {

  // Clear the mesh
  aMesh->Clear();
  int faceCount = mesh->GetNumberOfCells();
  int vertexCount = mesh->GetNumberOfPoints();

  double p[3];
  for (int i = 0; i < mesh->GetNumberOfPoints(); i++) {
    mesh->GetPoint(i, p);
    aMesh->mVertices.push_back(Vector3(p[0], p[1], p[2]));
  }

  const vtkIdType *vertices;
  vtkIdType nVertices;
  mesh->BuildCells();
  for (int i = 0; i < mesh->GetNumberOfCells(); i++) {
    mesh->GetCellPoints(i, nVertices, vertices);
    if ( nVertices != 3 ) continue;
    for (int j = 0; j < 3; j++) {
      aMesh->mIndices.push_back(vertices[j]);
    }
  }

  vtkDataArray *pointData = mesh->GetPointData()->GetScalars();
  if (!pointData) {
	  return;
  }

  aMesh->attributesName = pointData->GetName();

  for (int i = 0; i < mesh->GetNumberOfPoints(); i++) {
    aMesh->mAttributes.push_back(Vector4(pointData->GetTuple1(i), 0, 0, 1));
  }


}

/// Import a VTK file from a file.
void Import_VTK(const char * aFileName, Mesh * aMesh) {

  // Open the VTK file
  vtkPolyDataReader *reader = vtkPolyDataReader::New();
  reader->SetFileName(aFileName);
  reader->Update();
  Import_PolyData( reader->GetOutput(), aMesh );
}

/// Import a VTP file from a file.
void Import_VTP(const char * aFileName, Mesh * aMesh) {

  // Open the VTK file
  vtkXMLPolyDataReader *reader = vtkXMLPolyDataReader::New();
  reader->SetFileName(aFileName);
  reader->Update();
  Import_PolyData( reader->GetOutput(), aMesh );
}

#else

/// Import a VTK file from a file.
void Import_VTK(const char * aFileName, Mesh * aMesh) {
	std::cout << "error : vtk is not included in this library" << std::endl;
	exit(1);
}

/// Import a VTP file from a file.
void Import_VTP(const char * aFileName, Mesh * aMesh) {
	std::cout << "error : vtk is not included in this library" << std::endl;
	exit(1);
}

#endif

#endif // __PLY_H_
