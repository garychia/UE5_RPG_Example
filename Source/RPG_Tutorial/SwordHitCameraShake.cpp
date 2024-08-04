// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordHitCameraShake.h"

USwordHitCameraShake::USwordHitCameraShake()
{
	this->OscillationDuration = .15f;

	this->RotOscillation.Pitch.Amplitude = 1.2f;
	this->RotOscillation.Pitch.Frequency = 50.f;

	this->LocOscillation.X.Amplitude = 1.f;
	this->LocOscillation.X.Frequency = 50.f;
	this->LocOscillation.X.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
	
	this->LocOscillation.Y.Amplitude = 1.f;
	this->LocOscillation.Z.Frequency = 50.f;
	
	this->LocOscillation.Z.Amplitude = 1.2f;
	this->LocOscillation.Z.Frequency = 50.f;
}