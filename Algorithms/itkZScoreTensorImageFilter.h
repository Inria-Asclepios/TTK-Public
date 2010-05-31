#ifndef _itk_ZScoreTensorImageFilter_h_
#define _itk_ZScoreTensorImageFilter_h_

#include <itkImageToImageFilter.h>

namespace itk
{


	template<class TInputImage, class TOutputImage>
	class ITK_EXPORT ZScoreTensorImageFilter:
		public ImageToImageFilter<TInputImage, TOutputImage>
	{
	

	public:

		typedef ZScoreTensorImageFilter Self;
		typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;

		typedef SmartPointer<Self> Pointer;
		typedef SmartPointer<const Self> ConstPointer;

		itkNewMacro(Self);
		itkTypeMacro(ZScoreTensorImageFilter, ImageToImageFilter);


		typedef TInputImage InputImageType;
		typedef TOutputImage OutputImageType;

		typedef typename InputImageType::PixelType InputPixelType;
		typedef typename OutputImageType::PixelType OutputPixelType;
		typedef typename OutputImageType::RegionType    OutputImageRegionType;


		typedef typename InputPixelType::ValueType ScalarType;

		itkStaticConstMacro( ImageDimension, unsigned int, InputImageType::ImageDimension);

		itkStaticConstMacro( TensorDegreesOfFreedom, unsigned int,  InputPixelType::DegreesOfFreedom);

		itkStaticConstMacro (CovarianceMatrixDOF, unsigned int, InputPixelType::DegreesOfFreedom*(InputPixelType::DegreesOfFreedom+1)/2);

		typedef  Tensor<ScalarType, TensorDegreesOfFreedom> CovMatrixType;
		typedef  Image <CovMatrixType, ImageDimension>    CovMatrixImageType;

		typedef Vector<ScalarType, InputPixelType::DegreesOfFreedom> VectorType;

		itkSetObjectMacro(CovarianceMatrixImage, CovMatrixImageType);
		itkGetObjectMacro(CovarianceMatrixImage, CovMatrixImageType);

	
		itkSetObjectMacro(AverageTensorImage, InputImageType);
		itkGetObjectMacro(AverageTensorImage, InputImageType);



	protected:

		ZScoreTensorImageFilter()
		{
			m_CovarianceMatrixImage = 0;
			m_AverageTensorImage = 0;
		}
		~ZScoreTensorImageFilter(){};

		void BeforeThreadedGenerateData(void);
		void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
		void PrintSelf(std::ostream& os, Indent indent) const
		{
			Superclass::PrintSelf(os,indent);
		}

		VectorType Tensor2Vec(const InputPixelType&) const;

	private:

		typename CovMatrixImageType::Pointer m_CovarianceMatrixImage;
		typename InputImageType::Pointer     m_AverageTensorImage;

	
	};



} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkZScoreTensorImageFilter.txx"
#endif


#endif
