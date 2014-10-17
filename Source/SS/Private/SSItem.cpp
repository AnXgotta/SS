

#include "SS.h"
#include "SSItem.h"


ASSItem::ASSItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}





////////////////////////////////////////////////////
//  Accessor/Mutator

FPropertiesStruct ASSItem::GetItemProperties(){
	return ItemProperties;
}


///////////////////////////////////////////////////
// Funcitonality

void ASSItem::OnAddedToContainer(){

}

void ASSItem::OnRemovedFromContainer(){

}