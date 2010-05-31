#include "itkVTKStructuredPointsTensorImageIO.h"

#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkStructuredPoints.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>


namespace itk
{
  
  void VTKStructuredPointsTensorImageIO::Read()
  {
    
    if(m_FileName == "")
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: filename is not set.");

	    
    // actually read the file
    vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
    reader->SetFileName( m_FileName.c_str() );

	if( !reader->IsFileStructuredPoints() )
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: File is not a VTK structured points.");

    vtkStructuredPoints* tensors = reader->GetOutput();
    tensors->Update();
    
    // Allocate the output image
    if( m_Output )
      m_Output->Initialize();
    
    TensorImageType::SizeType size;
    double spacing[3];
    double origin[3];
    for(unsigned int i=0; i<3; i++)
    {
      size[i] = tensors->GetDimensions()[i];
      spacing[i] = tensors->GetSpacing()[i];
      origin[i] = tensors->GetOrigin()[i];
    }
    
    TensorImageType::RegionType region;
    TensorImageType::IndexType index = {{0,0,0}};
    region.SetSize(size);
    region.SetIndex(index);
    
    try
    {
      m_Output->SetLargestPossibleRegion(region);
      m_Output->SetBufferedRegion(region);
      m_Output->SetRequestedRegion(region);
      m_Output->SetSpacing(spacing);
      m_Output->SetOrigin(origin);
      m_Output->Allocate();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error during memory allocation");
    }
    

    vtkDoubleArray* data = (vtkDoubleArray*)(tensors->GetPointData()->GetTensors());
    
    IteratorType it(m_Output,m_Output->GetLargestPossibleRegion());

    unsigned int ind=0;
    
    while( !it.IsAtEnd() )
    {
      
      double coefs[9];
      data->GetTuple(ind,coefs);
      
      double newCoefs[6];
      newCoefs[0] = coefs[0];
      newCoefs[1] = coefs[3];
      newCoefs[2] = coefs[4];
      newCoefs[3] = coefs[6];
      newCoefs[4] = coefs[7];
      newCoefs[5] = coefs[8];
      
      TensorType T(newCoefs);
      it.Set (T);
      
      ++ind;
      ++it;
      
    }
    
    reader->Delete();
  }

  
  void VTKStructuredPointsTensorImageIO::Write()
  {
    
    if(!this->CheckExtension(m_FileName.c_str()))
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Extension is not recognized.");
    
    if(m_Input.IsNull())
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Trying to write a null pointer.");

    
    TensorImageType::SizeType size = m_Input->GetLargestPossibleRegion().GetSize();
    int numVoxels = 1;
    int dims[3];
    double origin[3];
    double spacing[3];
    
    for(unsigned int i=0; i<3; i++)
    {
      numVoxels *= size[i];
      dims[i] = size[i];
      origin[i] = m_Input->GetOrigin()[i];
      spacing[i] = m_Input->GetSpacing()[i];
    }
    

    vtkDoubleArray* data = vtkDoubleArray::New();
    data->SetNumberOfComponents(9);
    data->SetNumberOfTuples(numVoxels);
    
    IteratorType it(m_Input,m_Input->GetLargestPossibleRegion());

    unsigned int ind=0;
    
    while( !it.IsAtEnd() )//for(int i=0;i<numVoxels;i++)
    {
      TensorType T = it.Get();
      double buffer[9];
      buffer[0] = T.GetNthComponent(0);
      buffer[1] = T.GetNthComponent(1);
      buffer[2] = T.GetNthComponent(3);
      buffer[3] = T.GetNthComponent(1);
      buffer[4] = T.GetNthComponent(2);
      buffer[5] = T.GetNthComponent(4);
      buffer[6] = T.GetNthComponent(3);
      buffer[7] = T.GetNthComponent(4);
      buffer[8] = T.GetNthComponent(5);
      
      data->SetTuple(ind,buffer);
      ++ind;
      ++it;
    }
    
    vtkStructuredPoints* tensors = vtkStructuredPoints::New();
    tensors->SetDimensions(dims);
    tensors->SetSpacing(spacing);
    tensors->SetOrigin(origin);
    tensors->GetPointData()->SetTensors(data);
    data->Delete();
    tensors->Update();

    vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
    writer->SetFileTypeToBinary();
    writer->SetFileName( m_FileName.c_str() );
    writer->SetInput(tensors);
    writer->Write();

    tensors->Delete();
    
  }
  
  bool VTKStructuredPointsTensorImageIO::CheckExtension(const char* name)
  {
    std::string filename = name;
    if(filename == "")
      return false;

    bool extensionFound =  false;
    std::string::size_type vtkPos = filename.rfind(".vtk");
    if ( (vtkPos != std::string::npos) && (vtkPos == filename.length()-4) )
      extensionFound = true;

    return extensionFound;
    
  }
  

} // end of namespace
