#ifndef _CUSTOMKERNEL_H___
#define _CUSTOMKERNEL_H___

#include "lib/common.h"
#include "kernel/Kernel.h"
#include "features/Features.h"

class CCustomKernel: public CKernel
{
 public:
  CCustomKernel();
  ~CCustomKernel() ;
  
  virtual bool init(CFeatures* l, CFeatures* r, bool do_init);
  virtual void cleanup();

  /// load and save kernel init_data
  virtual bool load_init(FILE* src);
  virtual bool save_init(FILE* dest);

  // return what type of kernel we are Linear,Polynomial, Custom,...
  inline virtual EKernelType get_kernel_type() { return K_CUSTOM; }

  /** return feature type the kernel can deal with
  */
  inline virtual EFeatureType get_feature_type()
  {
	  return F_ANY;
  }

  /** return feature class the kernel can deal with
  */
  inline virtual EFeatureClass get_feature_class()
  {
	  return C_ANY;
  }

  // return the name of a kernel
  virtual const CHAR* get_name() { return "Custom"; }

  // set kernel matrix (only elements from main diagonal and above)
  // from elements of maindiagonal and above (concat'd)
  bool set_kernel_matrix_diag(const REAL* m, int num);

  // set kernel matrix (only elements from main diagonal and above)
  // from squared matrix
  bool set_kernel_matrix(const REAL* m, int num);

 protected:
  /// compute kernel function for features a and b
  /// idx_{a,b} denote the index of the feature vectors
  /// in the corresponding feature object
  inline virtual REAL compute(INT idx_a, INT idx_b)
  {
	  if (idx_a < idx_b)
		  return kmatrix[idx_a*num_cols-idx_b*(idx_b+1)/2];
	  else
		  return kmatrix[idx_b*num_cols-idx_a*(idx_a+1)/2];
  }
  /*    compute_kernel*/

 protected:
  SHORTREAL* kmatrix;
  INT num_cols;
};
#endif
