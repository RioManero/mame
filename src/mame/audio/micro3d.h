// license:BSD-3-Clause
// copyright-holders:Philip Bennett
/*************************************************************************

     Microprose Games 3D hardware

*************************************************************************/
#ifndef MAME_AUDIO_MICRO3D_H
#define MAME_AUDIO_MICRO3D_H

#pragma once

class micro3d_sound_device : public device_t, public device_sound_interface
{
public:
	micro3d_sound_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	void dac_w(u8 data) { m_dac_data = data; }
	void noise_sh_w(u8 data);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// sound stream update overrides
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

private:
	enum dac_registers {
		VCF,
		VCQ,
		VCA,
		PAN
	};

	struct biquad
	{
		double a0, a1, a2;      /* Numerator coefficients */
		double b0, b1, b2;      /* Denominator coefficients */
	};

	struct lp_filter
	{
		void init(double fs);
		void recompute(double k, double q, double fc);

		std::unique_ptr<float[]> history;
		std::unique_ptr<float[]> coef;
		double fs;
		biquad proto_coef[2];
	};

	struct m3d_filter_state
	{
		void configure(double r, double c);

		double      capval;
		double      exponent;
	};

	u8      m_dac_data;

	u8      m_dac[4];

	float   m_gain;
	u32     m_noise_shift;
	u8      m_noise_value;
	u8      m_noise_subcount;

	m3d_filter_state    m_noise_filters[4];
	lp_filter           m_filter;
	sound_stream        *m_stream;
};

DECLARE_DEVICE_TYPE(MICRO3D, micro3d_sound_device)

#endif // MAME_AUDIO_MICRO3D_H