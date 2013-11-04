#ifndef _itkMultiResolutionPDEDeformableRegistration2Tensor_txx
#define _itkMultiResolutionPDEDeformableRegistration2Tensor_txx
#include "itkMultiResolutionPDEDeformableRegistration2Tensor.h"

#include "itkImageRegionIterator.h"
#include "vnl/vnl_math.h"

#include "itkVectorLinearInterpolateNearestNeighborExtrapolateImageFunction.h"

namespace itk
{

/*
 * Default constructor
 */
template < class TFixedImage, class TMovingImage, class TDisplacementField,
    class TSolverPrecision >
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage, TMovingImage,
      TDisplacementField, TSolverPrecision>::MultiResolutionPDEDeformableRegistration2Tensor()
  {

    this->SetNumberOfRequiredInputs(2);

    typename DefaultRegistrationType::Pointer registrator =
        DefaultRegistrationType::New();
    m_RegistrationFilter
        = static_cast<RegistrationType*> (registrator.GetPointer());

    m_MovingImagePyramid = MovingImagePyramidType::New();
    m_FixedImagePyramid = FixedImagePyramidType::New();
    m_FieldExpander = FieldExpanderType::New();

    m_NumberOfLevels = 3;
    m_NumberOfIterations.resize(m_NumberOfLevels);
    m_FixedImagePyramid->SetNumberOfLevels(m_NumberOfLevels);
    m_MovingImagePyramid->SetNumberOfLevels(m_NumberOfLevels);

    unsigned int ilevel;
    for (ilevel = 0; ilevel < m_NumberOfLevels; ilevel++)
      {
        m_NumberOfIterations[ilevel] = 10;
      }
    m_CurrentLevel = 0;

    m_StopRegistrationFlag = false;
  }

/*
 * Set the moving image image.
 */
template < class TFixedImage, class TMovingImage, class TDisplacementField,
    class TSolverPrecision >
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage, TMovingImage,
      TDisplacementField, TSolverPrecision>::SetMovingImage(
      const MovingImageType * ptr)
  {
    this->ProcessObject::SetNthInput(2, const_cast<MovingImageType *> (ptr));
  }

/*
 * Get the moving image image.
 */
template < class TFixedImage, class TMovingImage, class TDisplacementField,
    class TSolverPrecision >
const  typename MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::MovingImageType *
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::GetMovingImage(void) const
    {
      return dynamic_cast< const MovingImageType * >
      ( this->ProcessObject::GetInput( 2 ) );
    }

  /*
   * Set the fixed image.
   */
  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::SetFixedImage(
      const FixedImageType * ptr )
    {
      this->ProcessObject::SetNthInput( 1, const_cast< FixedImageType * >( ptr ) );
    }

  /*
   * Get the fixed image.
   */
  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  const typename MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::FixedImageType *
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::GetFixedImage(void) const
    {
      return dynamic_cast< const FixedImageType * >
      ( this->ProcessObject::GetInput( 1 ) );
    }

  /*
   *
   */
  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  std::vector<SmartPointer<DataObject> >::size_type
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::GetNumberOfValidRequiredInputs() const
    {
      typename std::vector<SmartPointer<DataObject> >::size_type num = 0;

      if (this->GetFixedImage())
        {
          num++;
        }

      if (this->GetMovingImage())
        {
          num++;
        }

      return num;
    }

  /*
   * Set the number of multi-resolution levels
   */
  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::SetNumberOfLevels(
      unsigned int num )
    {
      if( m_NumberOfLevels != num )
        {
          this->Modified();
          m_NumberOfLevels = num;
          m_NumberOfIterations.resize( m_NumberOfLevels );
        }

      if( m_MovingImagePyramid && m_MovingImagePyramid->GetNumberOfLevels() != num )
        {
          m_MovingImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
        }
      if( m_FixedImagePyramid && m_FixedImagePyramid->GetNumberOfLevels() != num )
        {
          m_FixedImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
        }
    }

  /*
   * Standard PrintSelf method.
   */
  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
      os << indent << "NumberOfLevels: " << m_NumberOfLevels << std::endl;
      os << indent << "CurrentLevel: " << m_CurrentLevel << std::endl;

      os << indent << "NumberOfIterations: [";
      unsigned int ilevel;
      for( ilevel = 0; ilevel < m_NumberOfLevels - 1; ilevel++ )
        {
          os << m_NumberOfIterations[ilevel] << ", ";
        }
      os << m_NumberOfIterations[ilevel] << "]" << std::endl;

      os << indent << "RegistrationFilter: ";
      os << m_RegistrationFilter.GetPointer() << std::endl;
      os << indent << "MovingImagePyramid: ";
      os << m_MovingImagePyramid.GetPointer() << std::endl;
      os << indent << "FixedImagePyramid: ";
      os << m_FixedImagePyramid.GetPointer() << std::endl;

      os << indent << "StopRegistrationFlag: ";
      os << m_StopRegistrationFlag << std::endl;
    }

  /*
   * Perform a the deformable registration using a multiresolution scheme
   * using an internal mini-pipeline
   *
   *  ref_pyramid ->  registrator  ->  field_expander --|| tempField
   * test_pyramid ->           |                              |
   *                           |                              |
   *                           --------------------------------
   *
   * A tempField image is used to break the cycle between the
   * registrator and field_expander.
   *
   */
  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::GenerateData()
    {

      // Check for NULL images and pointers
      MovingImageConstPointer movingImage = this->GetMovingImage();
      FixedImageConstPointer fixedImage = this->GetFixedImage();

      if( !movingImage || !fixedImage )
        {
          itkExceptionMacro( << "Fixed and/or moving image not set" );
        }

      if( !m_MovingImagePyramid || !m_FixedImagePyramid )
        {
          itkExceptionMacro( << "Fixed and/or moving pyramid not set" );
        }

      if( !m_RegistrationFilter )
        {
          itkExceptionMacro( << "Registration filter not set" );
        }

      // Create the image pyramids.
      m_MovingImagePyramid->SetInput( movingImage );
      m_MovingImagePyramid->UpdateLargestPossibleRegion();

      m_FixedImagePyramid->SetInput( fixedImage );
      m_FixedImagePyramid->UpdateLargestPossibleRegion();

      // Initializations
      m_CurrentLevel = 0;
      m_StopRegistrationFlag = false;

      unsigned int movingLevel = vnl_math_min( (int) m_CurrentLevel,
          (int) m_MovingImagePyramid->GetNumberOfLevels() );

      unsigned int fixedLevel = vnl_math_min( (int) m_CurrentLevel,
          (int) m_FixedImagePyramid->GetNumberOfLevels() );

      DisplacementFieldPointer tempField = NULL;
      bool lastShrinkFactorsAllOnes = false;

      while ( !this->Halt() )
        {

          if( m_CurrentLevel == 0 )
            {
              // TODO: What to do if there is an input deformation field?
              // Will need a VectorMultiResolutionPyramidImageFilter to downsample it.
              m_RegistrationFilter->SetInitialDisplacementField( NULL );

            }
          else
            {
              // Resample the field to be the same size as the fixed image
              // at the current level
	      typename FieldExpanderType::Pointer fieldExpander = FieldExpanderType::New();
              fieldExpander->SetInput( tempField );

              typename TensorImageType::Pointer fi =
              m_FixedImagePyramid->GetOutput( fixedLevel );
              fieldExpander->SetSize(
                  fi->GetLargestPossibleRegion().GetSize() );
              fieldExpander->SetOutputStartIndex(
                  fi->GetLargestPossibleRegion().GetIndex() );
              fieldExpander->SetOutputOrigin( fi->GetOrigin() );
              fieldExpander->SetOutputSpacing( fi->GetSpacing());
	      fieldExpander->SetOutputDirection( fi->GetDirection() );

              fieldExpander->UpdateLargestPossibleRegion();
              fieldExpander->SetInput( NULL );
              tempField = fieldExpander->GetOutput();
              tempField->DisconnectPipeline();

              m_RegistrationFilter->SetInitialDisplacementField( tempField );

            }

          // setup registration filter and pyramids
          m_RegistrationFilter->SetMovingImage( m_MovingImagePyramid->GetOutput(movingLevel) );
          m_RegistrationFilter->SetFixedImage( m_FixedImagePyramid->GetOutput(fixedLevel) );

          m_RegistrationFilter->SetNumberOfIterations( m_NumberOfIterations[m_CurrentLevel] );

          // cache shrink factors for computing the next expand factors.
          lastShrinkFactorsAllOnes = true;
          for( unsigned int idim = 0; idim < ImageDimension; idim++ )
            {
              if ( m_FixedImagePyramid->GetSchedule()[fixedLevel][idim] > 1 )
                {
                  lastShrinkFactorsAllOnes = false;
                  break;
                }
            }

          // compute new deformation field
          m_RegistrationFilter->UpdateLargestPossibleRegion();
          tempField = m_RegistrationFilter->GetOutput();
          tempField->DisconnectPipeline();

          // Increment level counter.
          m_CurrentLevel++;
          movingLevel = vnl_math_min( (int) m_CurrentLevel,
              (int) m_MovingImagePyramid->GetNumberOfLevels() );
          fixedLevel = vnl_math_min( (int) m_CurrentLevel,
              (int) m_FixedImagePyramid->GetNumberOfLevels() );

          // Invoke an iteration event.
          this->InvokeEvent( IterationEvent() );

          // We can release data from pyramid which are no longer required.
          if ( movingLevel > 0 )
            {
              m_MovingImagePyramid->GetOutput( movingLevel - 1 )->ReleaseData();
            }
          if( fixedLevel > 0 )
            {
              m_FixedImagePyramid->GetOutput( fixedLevel - 1 )->ReleaseData();
            }

        } // while not Halt()

      if( !lastShrinkFactorsAllOnes )
        {
          // Some of the last shrink factors are not one
          // graft the output of the expander filter to
          // to output of this filter

          // resample the field to the same size as the fixed image
	  typename FieldExpanderType::Pointer fieldExpander = FieldExpanderType::New();
          fieldExpander->SetInput( tempField );
          fieldExpander->SetSize(
              fixedImage->GetLargestPossibleRegion().GetSize() );
          fieldExpander->SetOutputStartIndex(
              fixedImage->GetLargestPossibleRegion().GetIndex() );
          fieldExpander->SetOutputOrigin( fixedImage->GetOrigin() );
          fieldExpander->SetOutputSpacing( fixedImage->GetSpacing());
	  fieldExpander->SetOutputDirection( fixedImage->GetDirection() );

          fieldExpander->UpdateLargestPossibleRegion();
          this->GraftOutput( fieldExpander->GetOutput() );
        }
      else
        {
          // all the last shrink factors are all ones
          // graft the output of registration filter to
          // to output of this filter
          this->GraftOutput( tempField );
        }

      // Release memory
      m_FieldExpander->SetInput( NULL );
      m_FieldExpander->GetOutput()->ReleaseData();
      m_RegistrationFilter->SetInput( NULL );
      m_RegistrationFilter->GetOutput()->ReleaseData();

    }

  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::StopRegistration()
    {
      m_RegistrationFilter->StopRegistration();
      m_StopRegistrationFlag = true;
    }

  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  bool
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::Halt()
    {
      // Halt the registration after the user-specified number of levels
      if (m_NumberOfLevels != 0)
        {
          this->UpdateProgress( static_cast<float>( m_CurrentLevel ) /
              static_cast<float>( m_NumberOfLevels ) );
        }

      if ( m_CurrentLevel >= m_NumberOfLevels )
        {
          return true;
        }

      if ( m_StopRegistrationFlag )
        {
          return true;
        }
      else
        {
          return false;
        }

    }

  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::GenerateOutputInformation()
    {

      typename DataObject::Pointer output;

      if( this->GetInput(0) )
        {
          // Initial deformation field is set.
          // Copy information from initial field.
          this->Superclass::GenerateOutputInformation();

        }
      else if( this->GetFixedImage() )
        {
          // Initial deforamtion field is not set.
          // Copy information from the fixed image.
          for (unsigned int idx = 0; idx <
              this->GetNumberOfOutputs(); ++idx )
            {
              output = this->GetOutput(idx);
              if (output)
                {
                  output->CopyInformation(this->GetFixedImage());
                }
            }

        }

    }

  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::GenerateInputRequestedRegion()
    {
      // call the superclass's implementation
      Superclass::GenerateInputRequestedRegion();

      // request the largest possible region for the moving image
      MovingImagePointer movingPtr =
      const_cast< MovingImageType * >( this->GetMovingImage() );
      if( movingPtr )
        {
          movingPtr->SetRequestedRegionToLargestPossibleRegion();
        }

      // just propagate up the output requested region for
      // the fixed image and initial deformation field.
      DisplacementFieldPointer inputPtr =
      const_cast< DisplacementFieldType * >( this->GetInput() );
      DisplacementFieldPointer outputPtr = this->GetOutput();
      FixedImagePointer fixedPtr =
      const_cast< FixedImageType *>( this->GetFixedImage() );

      if( inputPtr )
        {
          inputPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
        }

      if( fixedPtr )
        {
          fixedPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
        }

    }

  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::EnlargeOutputRequestedRegion(
      DataObject * ptr )
    {
      // call the superclass's implementation
      Superclass::EnlargeOutputRequestedRegion( ptr );

      // set the output requested region to largest possible.
      DisplacementFieldType * outputPtr;
      outputPtr = dynamic_cast<DisplacementFieldType*>( ptr );

      if( outputPtr )
        {
          outputPtr->SetRequestedRegionToLargestPossibleRegion();
        }

    }

  template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
  void
  MultiResolutionPDEDeformableRegistration2Tensor<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
  ::SetSchedule(const ScheduleType& schedule)
    {
      // This method is more powerful than SetNumberOfLevels(). This means that it will overwrite
      // the number of levels to match the desired schedule.
      // Hence we only have to check the column dimension of the schedule

      if( /*schedule.rows() != m_NumberOfLevels ||*/
          schedule.columns() != ImageDimension )
        {
          itkDebugMacro(<< "Schedule has wrong dimensions" );
          return;
        }

      this->SetNumberOfLevels (schedule.rows());

      m_FixedImagePyramid->SetSchedule (schedule);
      m_MovingImagePyramid->SetSchedule (schedule);

    }

} // end namespace itk

#endif
