// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCameraShake.h"

UCustomCameraShake::UCustomCameraShake()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.2f;
	OscillationBlendOutTime = 0.2f;

	LocOscillation.X.Amplitude = 2.0f;
	LocOscillation.X.Frequency = 50.0f;
	LocOscillation.X.InitialOffset = EOO_OffsetRandom;
	LocOscillation.X.Waveform = EOscillatorWaveform::SineWave;

	LocOscillation.Y.Amplitude = 2.0f;
	LocOscillation.Y.Frequency = 50.0f;
	LocOscillation.Y.InitialOffset = EOO_OffsetRandom;
	LocOscillation.Y.Waveform = EOscillatorWaveform::SineWave;

	LocOscillation.Z.Amplitude = 2.0f;
	LocOscillation.Z.Frequency = 50.0f;
	LocOscillation.Z.InitialOffset = EOO_OffsetRandom;
	LocOscillation.Z.Waveform = EOscillatorWaveform::SineWave;
}
