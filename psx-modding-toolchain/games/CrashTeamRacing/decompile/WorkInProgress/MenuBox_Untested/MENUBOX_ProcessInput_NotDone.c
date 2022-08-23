#include <common.h>

void DECOMP_MENUBOX_ProcessInput(struct MenuBox* m)
{
	struct MenuRow* currMenuRow;
	int button;
	int oldRow;
	int newRow;
	int i;
	
	#if 0
	// do we need this?
	FUN_8006c684();
	#endif
	
	if(
		((m->state & 4) == 0) &&
		
		// must be both 0x20000 and 0x40000
		((m->state & 0x60000) != 0x60000)
	  )
	{
	  oldRow = m->rowSelected;
	  
	  currMenuRow = m->rows[oldRow];
	  
	  if(sdata.activeSubMenu != m)
	  {
	    activeSubMenu = m;
	    
	    // if input should clear upon opening
	    if(m->state & 0x10000)
	    {
	  	  MenuBox_ClearInput();
	    }
	  }
	  
	  // button from any player
	  button = sdata.AnyPlayerTap;
	  
	  // if only P1 can use menu
	  if ((*(uint *)(param_1 + 8) & 0x8000) == 0) 
	  {
	    // get button from P1
	    button = sdata.buttonTapPerPlayer[0];
	  }
	  
	  // if P1 is not holding L1 or R1
	  if((sdata.buttonHeldPerPlayer[0] & 0xc00) != 0) goto AfterButtons;
	  
	  // If nobody pressed D-Pad, Cross, Square, Triangle, Circle
	  if ((button & 0x4007f) == 0) goto AfterButtons;
	  
	  // optimized way to check all four button presses:
	  // up, down, left, right, and get new row
	  for(i = 0; i < 4; i++)
	  {
		if(((button>>i) & 1) != 0) 
		{
			newRow = *(char*)((char*)&currMenuRow->rowOnPressUp + i);
			break;
		}
	  }
	  
	  // check if row has changed
	  if(oldRow != newRow)
	  {
	    // if cursor moving sound is not muted
	    if((m->state & 0x800000) == 0)
	    {
	  	  OtherFX_Play(0,1);
	    }
	  }
	  
	  // if not Cross or Circle
	  if((button & 0x50) == 0)
	  {
	    // process GO BACK
	  }
	  
	  // if Cross or Circle
	  else
	  {
	    // process ENTER
	  }
	  
	  MenuBox_ClearInput();
	  
	  // this is why you can bug character select
	  // by pressing Down and X at the same time,
	  // cause the row isn't set until after X is processed
	  m->selectedRow = newRow;
	  
	}
	
	// why does it do this?
	// is this a mistake?
	// why not just call the function once for one hierarchy level
	if(m->state & 0x10)
	{
		m->ptrNextMenuBox_InHierarchy->ptrPrevMenuBox_InHierarchy = m;
		
		// call yourself
		DECOMP_MENUBOX_ProcessInput(m->ptrNextMenuBox_InHierarchy);
	}
}