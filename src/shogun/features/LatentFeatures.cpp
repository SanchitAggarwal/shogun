/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2012 Viktor Gal
 * Copyright (C) 2012 Viktor Gal
 */

#include <shogun/features/LatentFeatures.h>

using namespace shogun;

CLatentFeatures::CLatentFeatures ()
{
  init ();
}

CLatentFeatures::CLatentFeatures (int32_t num_samples)
{
  init ();
  m_samples = new CDynamicObjectArray<CLatentData> (num_samples);
  SG_REF (m_samples);
}

CLatentFeatures::~CLatentFeatures ()
{
  SG_UNREF (m_samples);
}

CFeatures* CLatentFeatures::duplicate () const
{
  return new CLatentFeatures (*this);
}

EFeatureType CLatentFeatures::get_feature_type () const
{
  return F_ANY;
}

EFeatureClass CLatentFeatures::get_feature_class () const
{

  return C_LATENT;
}


int32_t CLatentFeatures::get_num_vectors () const
{
  if (m_samples == NULL)
    return 0;
  else
    return m_samples->get_num_elements ();
}

int32_t CLatentFeatures::get_size () const
{
  return sizeof (float64_t);
}

bool CLatentFeatures::add_sample (CLatentData* example)
{
  ASSERT (m_samples != NULL);
  m_samples->push_back (example);

}

CLatentData* CLatentFeatures::get_sample (index_t idx)
{
  ASSERT (m_samples != NULL);
  if (idx < 0 || idx >= this->get_num_vectors ())
    SG_ERROR("Out of index!\n");

  return (CLatentData*) m_samples->get_element (idx);

}

void CLatentFeatures::init ()
{
  m_parameters->add ((CSGObject**) &m_samples, "samples", "Array of examples");
}

