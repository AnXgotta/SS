// Fill out your copyright notice in the Description page of Project Settings.

#include "SS.h"
#include "SSInteractable.h"
#include "SSConstants.h"


USSInteractable::USSInteractable(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}


void ISSInteractable::OnRecognized(){
	USSConstants::ScreenMessage("OnRecognized - Base (interface)", 5.0f, FColor::Yellow);
}

void ISSInteractable::OnNotRecognized(){
	USSConstants::ScreenMessage("OnNotRecognized - Base (interface)", 5.0f, FColor::Yellow);
}


void ISSInteractable::OnInteract(){
	USSConstants::ScreenMessage("OnInteract - Base (interface)", 5.0f, FColor::Yellow);
}