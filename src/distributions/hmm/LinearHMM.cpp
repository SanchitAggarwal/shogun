#include "distributions/hmm/LinearHMM.h"
#include "lib/common.h"
#include "features/WordFeatures.h"
#include "lib/io.h"

#include <assert.h>

CLinearHMM::CLinearHMM(CWordFeatures* f) : hist(NULL), log_hist(NULL), features(f)
{
	sequence_length = f->get_num_features();
	num_symbols     = f->get_num_symbols();
	num_params      = sequence_length*num_symbols;
}

CLinearHMM::CLinearHMM(INT p_num_features, INT p_num_symbols) : hist(NULL), log_hist(NULL)
{
	sequence_length = p_num_features;
	num_symbols     = p_num_symbols;
	num_params      = sequence_length*num_symbols;
}

CLinearHMM::~CLinearHMM()
{
	delete[] hist;
	delete[] log_hist;
}

bool CLinearHMM::train()
{
	delete[] hist;
	delete[] log_hist;
	INT* int_hist = new int[num_params];
	assert(int_hist);

	INT vec;
	INT i;

	for (i=0; i< num_params; i++)
		int_hist[i]=0;

	for (vec=0; vec<features->get_num_vectors(); vec++)
	{
		INT len;
		bool to_free;

		WORD* vector=((CWordFeatures*) features)->get_feature_vector(vec, len, to_free);

		//just count the symbols per position -> histogram
		//
		for (INT feat=0; feat<len ; feat++)
			int_hist[feat*num_symbols+vector[feat]]++;

		((CWordFeatures*) features)->free_feature_vector(vector, vec, to_free);
	}

	//trade memory for speed
	hist= new REAL[num_params];
	log_hist= new REAL[num_params];

	assert(hist);
	assert(log_hist);

	for (i=0;i<sequence_length;i++)
	{
		for (INT j=0; j<num_symbols; j++)
		{
			REAL sum=0;
			for (INT k=0; k<features->get_original_num_symbols(); k++)
			{
				sum+=int_hist[i*num_symbols+features->get_masked_symbols((WORD)j,(BYTE) 254)+k];
			}

			hist[i*num_symbols+j]=(int_hist[i*num_symbols+j]+pseudo_count)/(sum+features->get_original_num_symbols()*pseudo_count);
			log_hist[i*num_symbols+j]=log(hist[i*num_symbols+j]);
		}
	}

	delete[] int_hist;
	return true;
}

bool CLinearHMM::train(const INT* indizes, INT num_indizes, REAL pseudo_count)
{
	delete[] hist;
	delete[] log_hist;
	INT* int_hist = new int[num_params];
	assert(int_hist);

	INT vec;
	INT i;

	for (i=0; i< num_params; i++)
		int_hist[i]=0;

	for (vec=0; vec<num_indizes; vec++)
	{
		INT len;
		bool to_free;

		assert(indizes[vec]>=0 && indizes[vec]<((CWordFeatures*) features)->get_num_vectors());
		WORD* vector=((CWordFeatures*) features)->get_feature_vector(indizes[vec], len, to_free);

		//just count the symbols per position -> histogram
		//
		for (INT feat=0; feat<len ; feat++)
			int_hist[feat*num_symbols+vector[feat]]++;

		((CWordFeatures*) features)->free_feature_vector(vector, indizes[vec], to_free);
	}

	//trade memory for speed
	hist= new REAL[num_params];
	log_hist= new REAL[num_params];

	assert(hist);
	assert(log_hist);

	for (i=0;i<sequence_length;i++)
	{
		for (INT j=0; j<num_symbols; j++)
		{
			REAL sum=0;
			for (INT k=0; k<features->get_original_num_symbols(); k++)
			{
				sum+=int_hist[i*num_symbols+features->get_masked_symbols((WORD)j,(BYTE) 254)+k];
			}

			hist[i*num_symbols+j]=(int_hist[i*num_symbols+j]+pseudo_count)/(sum+features->get_original_num_symbols()*pseudo_count);
			log_hist[i*num_symbols+j]=log(hist[i*num_symbols+j]);
		}
	}

	delete[] int_hist;
	return true;
}

REAL CLinearHMM::get_log_likelihood_example(WORD* vector, INT len)
{
	REAL result=log_hist[vector[0]];

	for (INT i=1; i<len; i++)
		result+=log_hist[i*num_symbols+vector[i]];
	
	return result;
}

REAL CLinearHMM::get_log_likelihood_example(INT num_example)
{
	INT len;
	bool to_free;
	WORD* vector=((CWordFeatures*) features)->get_feature_vector(num_example, len, to_free);
	REAL result=log_hist[vector[0]];
	for (INT i=1; i<len; i++)
		result+=log_hist[i*num_symbols+vector[i]];
	((CWordFeatures*) features)->free_feature_vector(vector, num_example, to_free);
	return result;
}

REAL CLinearHMM::get_likelihood_example(WORD* vector, INT len)
{
	REAL result=hist[vector[0]];

	for (INT i=1; i<len; i++)
		result*=hist[i*num_symbols+vector[i]];
	
	return result;
}

REAL CLinearHMM::get_log_derivative(INT param_num, INT num_example)
{
	INT len;
	bool to_free;
	WORD* vector=((CWordFeatures*) features)->get_feature_vector(num_example, len, to_free);
	REAL result=0;
	INT position=param_num/num_symbols;
	if (vector[position] == param_num/position)
			result=1.0/hist[param_num];

	((CWordFeatures*) features)->free_feature_vector(vector, num_example, to_free);
	return result;
}

void CLinearHMM::set_log_hist(REAL* new_log_hist)
{
	if (!log_hist)
		log_hist = new REAL[num_params];

	if (!hist)
		hist = new REAL[num_params];

	for (INT i=0; i< num_params; i++)
	{
		log_hist[i]=new_log_hist[i];
		hist[i]=exp(log_hist[i]);
	}
}

void CLinearHMM::set_hist(REAL* new_hist)
{
	if (!log_hist)
		log_hist = new REAL[num_params];

	if (!hist)
		hist = new REAL[num_params];

	for (INT i=0; i< num_params; i++)
	{
		hist[i]=new_hist[i];
		log_hist[i]=log(hist[i]);
	}
}
