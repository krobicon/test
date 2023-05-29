	    if (!level->isPlayable() && counter % 1900 == 0) {
		    if (findMatch) {
			    printf("finding \n");
		    }
		    else {
				int k = 0;
			    	int j = 0;
				while (k < 125) {
					simInput->emit(EV_REL, REL_Y, 16);
					simInput->emit(EV_REL, REL_X, -16);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					k++;
					usleep(5000);
				}
			 	while (j < 11) {
					simInput->emit(EV_REL, REL_Y, -8);
					simInput->emit(EV_REL, REL_X, 8);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					j++;
					usleep(5000);
				}
			    	simInput->click();
			    	findMatch = true;	
		    }
	    }

if (localPlayer->isDead() && counter % 1000 == 0) {
			        int k = 0;
			    	int j = 0;
			    	int p = 0;
				simInput->emit(EV_KEY, KEY_SPACE, 1);
				simInput->emit(EV_SYN, SYN_REPORT, 0);
				simInput->emit(EV_KEY, KEY_SPACE, 0);
				simInput->emit(EV_SYN, SYN_REPORT, 0);
				usleep(10000);
			    	while (k < 125) {
					simInput->emit(EV_REL, REL_Y, 15);
					simInput->emit(EV_REL, REL_X, -15);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					k++;
					usleep(10000);
				}
			    	while (j < 120) {
					simInput->emit(EV_REL, REL_Y, -3);
					simInput->emit(EV_REL, REL_X, 7);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					j++;
					usleep(10000);
				}
			    	simInput->click();
			    	usleep(10000);
			    	while (p < 66) {
					simInput->emit(EV_REL, REL_Y, 4);
					simInput->emit(EV_REL, REL_X, 1);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					p++;
					usleep(100000);
				}
			    	simInput->click();
			    	usleep(300000);
			    	simInput->click();
			    	usleep(300000);
			    	simInput->click();
		    }


			findMatch = false;
			if (counter % 2500 == 0 && localPlayer->isGrounded())
				simInput->click();
