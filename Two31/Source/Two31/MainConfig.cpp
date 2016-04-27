// Fill out your copyright notice in the Description page of Project Settings.

#include "Two31.h"
#include "MainConfig.h"

UMainConfig* UMainConfig::Instance = new UMainConfig();

UMainConfig::UMainConfig()
{
	LoadConfig();
}

UMainConfig::~UMainConfig()
{
	SaveConfig();
}