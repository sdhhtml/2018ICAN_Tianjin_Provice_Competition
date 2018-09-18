	if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			//printf("我是你爸爸");
			//printf("%d",len);
			x=y=0;
			len = len - 1;
			for(t=0;t<len;t++)
			{
				if(USART_RX_BUF[t] == ' ')      //找到界定符的话
				{
					unsigned char i = 0;
					for(i = 0;i<t; i++)
					{
						uint16_t m = 1;
						unsigned char k = 0;
						for(k = 0; k<(t-i-1);k++)
							m = m*10;
						x = x + (USART_RX_BUF[i] - '0')*m;
					}
					for(i = t+1;i<len; i++)
					{
						uint16_t m = 1;
						unsigned char k = 0;
						for(k = 0; k<(len-1-i);k++)
							m = m*10;
						y = y + (USART_RX_BUF[i] - '0')*m;
					}
					last_last_status = last_status;       //上上次红绿灯状态。
					last_status = status;                 //上次红绿灯的状态。
					status = USART_RX_BUF[len] - '0';
					LCD_ShowNum(85,160,x,3,16);           //显示x坐标
					LCD_ShowNum(85,180,y,3,16);           //显示y坐标
					LCD_ShowNum(85,200,status,3,16);           //显示当前状态
				}
			}
			USART_RX_STA=0;
		}