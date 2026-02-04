//			printf("Text Running!\r\n");
			
//			/*速度环控制*/
//			if(mode == 1)//如果模式1则单环控制
//			{
//				pid_SpeedCtrl.Target = 2000.0f;//目标值
//			}
//			pid_SpeedCtrl.Actual = (float)Speed;//更新实际值
//			
//			PID_Calc(&pid_SpeedCtrl);//获取out值
//			
//			tx_data[0] = (uint16_t)pid_SpeedCtrl.Out >> 8;
//			tx_data[1] = (uint16_t)pid_SpeedCtrl.Out;
//			
//			CAN_TxMsg(0x201,tx_data,1);
//			
//			if(mode == 2)//如果模式2则双环控制
//			{
//				/*位置环控制*/
//				pid_LocationCtrl.Target = 2000.0f;//目标值
//				pid_LocationCtrl.Actual = (float)Angel;//更新实际值			
//				
//					/*电机过零处理*/
//				if(pid_LocationCtrl.Target - pid_LocationCtrl.Actual > 4096)
//				{
//					pid_LocationCtrl.Actual += 8191; 
//				}
//				else if(pid_LocationCtrl.Target - pid_LocationCtrl.Actual < -4096)
//				{
//					pid_LocationCtrl.Actual -= 8191; 
//				}
//				else
//				{
//					
//				}
//				
//				PID_Calc(&pid_LocationCtrl);//获取out值
//				
//				pid_SpeedCtrl.Target = (uint16_t)pid_LocationCtrl.Out;
