/***************************************************************************
  This file is part of Project Apollo - NASSP
  Copyright 2004-2005 Jean-Luc Rocca-Serra, Mark Grant

  ORBITER vessel module: LEM virtual cockpit code

  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************** Revision History ****************************
  *	$Log$
  *	Revision 1.4  2005/08/14 16:08:20  tschachim
  *	LM is now a VESSEL2
  *	Changed panel restore mechanism because the CSM mechanism
  *	caused CTDs, reason is still unknown.
  *	
  *	Revision 1.3  2005/08/10 21:54:04  movieman523
  *	Initial IMU implementation based on 'Virtual Apollo' code.
  *	
  *	Revision 1.2  2005/08/09 09:14:31  tschachim
  *	Introduced toggleswitch lib
  *	
  *	Revision 1.1  2005/02/11 12:54:07  tschachim
  *	Initial version
  *	
  **************************************************************************/

#include "Orbitersdk.h"
#include "stdio.h"
#include "math.h"
#include "OrbiterSoundSDK3.h"
#include "resource.h"

#include "nasspdefs.h"
#include "nasspsound.h"

#include "soundlib.h"
#include "toggleswitch.h"
#include "apolloguidance.h"
#include "LEMcomputer.h"
#include "dsky.h"
#include "IMU.h"

#include "landervessel.h"
#include "sat5_lmpkd.h"

void sat5_lmpkd::SetView() {


	//
	// Set camera offset
	//
	if (InVC) {
		switch (viewpos) {
		case LMVIEW_CDR:
			SetCameraOffset (_V(-0.68, 1.65, 1.35));
			break;

		case LMVIEW_LMP:
			SetCameraOffset (_V(0.92, 1.65, 1.23));
			break;
		}

	} else {
		if (InPanel && PanelId == LMPANEL_RNDZWINDOW) {
			SetCameraOffset (_V(0, -1.8, 0.55));
		} else {
			SetCameraOffset (_V(0, 0, 0));
		}
	}

	//
	// Change FOV for the LPD window 
	//
	if (InPanel && PanelId == LMPANEL_LPDWINDOW) {
	   // if this is the first time we've been here, save the current FOV
		if (InFOV) {
			SaveFOV = oapiCameraAperture();
			InFOV = false;
		}
		//set FOV to 70 degrees
		oapiCameraSetAperture(RAD * 35.0);

	} else {
		if(InFOV == false) {
			oapiCameraSetAperture(SaveFOV);
			InFOV = true;
		}
	}
}

bool sat5_lmpkd::clbkLoadVC (int id)

{
	switch (id) {
	case 0:
		SetCameraRotationRange(0.8 * PI, 0.8 * PI, 0.4 * PI, 0.4 * PI);
		InVC = true;
		InPanel = false;
		SetView();
		return true;

	default:
		return false;
	}
}

