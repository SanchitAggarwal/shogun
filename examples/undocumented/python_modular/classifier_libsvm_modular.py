#!/usr/bin/env python
traindat = '../data/fm_train_real.dat'
testdat = '../data/fm_test_real.dat'
label_traindat = '../data/label_train_twoclass.dat'

parameter_list = [[traindat,testdat,label_traindat,2.1,1,1e-5],[traindat,testdat,label_traindat,2.2,1,1e-5]]

def classifier_libsvm_modular (train_fname=traindat,test_fname=testdat,label_fname=label_traindat,width=2.1,C=1,epsilon=1e-5):
	from modshogun import RealFeatures, BinaryLabels
	from modshogun import GaussianKernel, LibSVM, CSVFile

	feats_train=RealFeatures(CSVFile(train_fname))
	feats_test=RealFeatures(CSVFile(test_fname))
	labels=BinaryLabels(CSVFile(label_fname))
	kernel=GaussianKernel(feats_train, feats_train, width)

	svm=LibSVM(C, kernel, labels)
	svm.set_epsilon(epsilon)
	svm.train()

	supportvectors = sv_idx=svm.get_support_vectors()
	alphas=svm.get_alphas()
	predictions = svm.apply(feats_test)
	#print predictions.get_labels()
	return predictions, svm, predictions.get_labels()

if __name__=='__main__':
	print('LibSVM')
	classifier_libsvm_modular(*parameter_list[0])
