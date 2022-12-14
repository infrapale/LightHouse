#define ADDR_LEN  MSG_ADDR_LEN+1
#define DATA_LEN  16
#define NBR_UNITS 2
#define NBR_RELAYS 4


char c;

char MsgDataBuff[DATA_LEN]; 
char MsgAddrBuff[ADDR_LEN]; 
char MsgType;
char Buff2[8];
boolean relay_on[NBR_UNITS][NBR_RELAYS];
short int MsgInBuffIndx =0;


void InitMySmart (void){
  byte i;
  byte j;
  MsgDataBuff[0]=0;
  MsgAddrBuff[0]=0;
  for (i=0;i<NBR_UNITS;i++) 
      for (j=0;j<NBR_RELAYS;j++) relay_on[i][j]=false;
 
  SerialFlag = false;
}




void GetMsg(void){
    MsgType = Smart.GetMsgType();
    Smart.CopyAddr(MsgAddrBuff);
    Smart.CopyData(MsgDataBuff);
    /*Serial.print("GetMsg(): ");
    Serial.print(MsgType); Serial.print(" ");
    Serial.print(MsgAddrBuff); Serial.print(" ");
    Serial.println(MsgDataBuff);
    */
    Smart.clear();
    msgReady = false;
    switch(MsgType){
       case SM_ADDR_MSG:
           HandleAddrMsg();
           break;   
       case SM_REPLY_ADDR:
           break;   
       case SM_BROADCAST:
           HandleBroadcastMsg();
           break;   
       case SM_QUICK:
           HandleQuickMsg();
           break; 
    }
}

void HandleAddrMsg(void){
  if (!strcmp(Me.addr, MsgAddrBuff)){
      if (!strcmp(MsgDataBuff,"?S")) PingReplyShort();
      if (!strcmp(MsgDataBuff,"?L")) PingReplyLong();
  }
}

void HandleBroadcastMsg(void){
  byte i=0;
  byte j;
  byte k;
  byte msg_indx = 0;
  byte action_indx = 0;
  boolean done = false;
  byte unit_indx; 
  byte relay_indx;  
  boolean relay_state;       
 

  while (! done){
      if (!strcmp(msg_table[i].msg, MsgAddrBuff)){
         done = true;
         if (msg_table[i].group == NODE_ADDR){
            msg_indx = i;
            action_indx = msg_table[i].msg_cat;
            //sub_indx = msg_table[i].tag1;        
         }
      }
      if (msg_table[++i].group == 0) done = true;
  }
  
  switch(action_indx){
      case TEMP_MSG:
          //strcpy(MsgAddrBuff, my_bcast[0].addr);
          strcpy(MsgDataBuff,"=");
          strcat(MsgDataBuff,"22.3C");
          Smart.Send(SM_REPLY_ADDR,MsgAddrBuff, MsgDataBuff);
          break;
      case 20:
          //strcpy(MsgAddrBuff, my_bcast[1].addr);
          strcpy(MsgDataBuff,"=");
          strcat(MsgDataBuff,"80%");
          Smart.Send(SM_REPLY_ADDR,MsgAddrBuff, MsgDataBuff);

          break;
      case RELAY_MSG:
          one_relay(msg_indx,0);
          break;
      case RELAY_GRP:
          i=msg_indx+1; done = false; 
          //sub_indx = msg_table[msg_indx].tag2;
          unit_indx = msg_table[i].unit_indx; 
          relay_indx = msg_table[i].sub_indx;  
          relay_state = relay_on[unit_indx-1][relay_indx-1] ; 
          //Serial.println("Relay group"); Serial.println (unit_indx); Serial.println (relay_indx); Serial.println (relay_state);                
          while(!done){
              //Serial.println(msg_table[i].msg);
              if ((msg_table[i].msg_cat == RELAY_MSG)){
                  if (relay_state)
                     one_relay(i,-1);
                  else
                     one_relay(i,1);   
                  i++;
              }
              else done= true;
          }
          break;   
      case 60: break;
      case 70: break;
  }
}

void one_relay(byte msg_indx, int state_ctrl){
    // state_ctrl == -1   switch off
    // state_ctrl ==  0   switch as defined in the message
    // state_ctrl == -1   switch on
    
    byte unit_indx;
    byte relay_indx;
    unit_indx = msg_table[msg_indx].unit_indx-1; 
    relay_indx = msg_table[msg_indx].sub_indx-1;
    switch(MsgDataBuff[0]){
        case '=':
            switch(MsgDataBuff[1]){
                case '0': relay_on[unit_indx][relay_indx] = false; break;
                case '1': relay_on[unit_indx][relay_indx] = true; break;
                case 'T': relay_on[unit_indx][relay_indx] = !relay_on[unit_indx][relay_indx]; break;
            }
            switch (state_ctrl){
                case -1: relay_on[unit_indx][relay_indx] = false; break;
                case 1:  relay_on[unit_indx][relay_indx] = true; break;
            }
            
            Buff2[0] = SM_QUICK;
            Buff2[1] = 'R';
            Buff2[2] = unit_indx+'1';
            Buff2[3] = relay_indx+'1';
            Buff2[4] = '=';
            if (relay_on[unit_indx][relay_indx]) Buff2[5] = '1'; else Buff2[5] = '0';
            Buff2[6] = 0;
            Serial.print("Buff2: "); Serial.println(Buff2);
            SendSoftCom(Buff2);
            break;
        case '?':
             
             break;
       }   


};

void HandleQuickMsg(void){   
    switch( MsgDataBuff[0]){
       case 'K':
          Serial.print("Key ");
          Serial.print(MsgDataBuff[1]);
          Serial.println(" pressed");
          break; 
    }
}

void AppendChar(char *toArray,char newChar){
    byte i=0;
    while (toArray[i]!=0) i++;
    toArray[i] = newChar;
    toArray[i+1] = 0;
}
void PingReplyShort(void){
   strcpy(MsgDataBuff, Me.name);  strcat(MsgDataBuff,",");
   strcat(MsgDataBuff, Me.system_sketch); 
   strcat(MsgDataBuff,",");
   AppendChar(MsgDataBuff,Me.index);
   Smart.Send(SM_REPLY_ADDR,MsgAddrBuff, MsgDataBuff);
}

void PingReplyLong(void){
   char s[8];
   strcpy(MsgDataBuff, Me.name);  strcat(MsgDataBuff,",");
   strcat(MsgDataBuff, Me.system_sketch);  strcat(MsgDataBuff,",");
   strcat(MsgDataBuff, Me.system_sketch); strcat(MsgDataBuff,",");
   AppendChar(MsgDataBuff,Me.index);strcat(MsgDataBuff,",");
   strcat(MsgDataBuff, Me.hardware_sketch); strcat(MsgDataBuff,",");
   strcat(MsgDataBuff, Me.software_sketch); strcat(MsgDataBuff,",");
   strcat(MsgDataBuff, Me.version); strcat(MsgDataBuff,":");
   itoa(MyTime.year,s,10);
   strcat(MsgDataBuff, s); strcat(MsgDataBuff,",");
   itoa(MyTime.month,s,10);
   strcat(MsgDataBuff, s); strcat(MsgDataBuff,",");
   itoa(MyTime.year,s,10);
   strcat(MsgDataBuff, s); strcat(MsgDataBuff,",");
   itoa(MyTime.day,s,10);
   strcat(MsgDataBuff, s); strcat(MsgDataBuff,"-");
   itoa(MyTime.hour,s,10);
   strcat(MsgDataBuff, s); strcat(MsgDataBuff,":");
   itoa(MyTime.minute,s,10);
   strcat(MsgDataBuff, s); strcat(MsgDataBuff,":");
   itoa(MyTime.second,s,10);
   strcat(MsgDataBuff, s); 
  
   Smart.Send(SM_REPLY_ADDR,MsgAddrBuff, MsgDataBuff);
}

void InterpretMsg(void){
    byte i;
    byte j;
    boolean done;
    //String s;
    char msgType; 
    boolean this_is_for_me = false;

    Smart.CopyData(MsgDataBuff);
    msgType = Smart.GetMsgType();
    switch(msgType){
       case SM_ADDR_MSG:
           Smart.CopyAddr(MsgAddrBuff);
           this_is_for_me = !strcmp(MsgAddrBuff, "MH2-1"); 
           
           break;   
       case SM_REPLY_ADDR:
           Smart.CopyAddr(MsgAddrBuff);
           break;   
       case SM_BROADCAST:
           Smart.CopyAddr(MsgAddrBuff);
           break;   
       case SM_QUICK:
           MsgAddrBuff[0]=0;
           break;   
    }

}
    
/*
    Serial.println(MsgDataBuff);
    switch( MsgInBuff.charAt(0)){ 
    case SM_ADDR_MSG:
       done =false; 
       if (MsgInBuff.substring(ADDR_MSG_START,ADDR_MSG_START+ADDR_MSG_LEN) == Me.name &&
          MsgInBuff.charAt(ADDR_MSG_DATA) == '.' &&
          MsgInBuff.charAt(ADDR_MSG_DATA+1) == Me.index){
          
          //<@Terminal.1PINGL>  <!Terminal.1 T129 T114 T129 16v01>
          if (!done && MsgInBuff.substring(ADDR_MSG_DATA+2,ADDR_MSG_DATA+3+ADDR_CMD_LEN) == PING_SHORT){
             PingReplyShort();
             done = true;    
           }
          if (!done && MsgInBuff.substring(ADDR_MSG_DATA+2,ADDR_MSG_DATA+3+ADDR_CMD_LEN) == PING_LONG){
             PingReplyLong();
             done = true;    
           }
           
        }
        //for ( i=1; i < 5; i++) if( MsgInBuff.charAt(i) !=uAddr[i-1]) b=false;
        break; 
    case SM_BROADCAST:
        done = false;
        // <#PINGS>
        if (!done && MsgInBuff.substring(BC_MSG_START,BC_MSG_START+BC_MSG_LEN) == PING_SHORT){
           delay(random(PING_DELAY_MIN, PING_DELAY_MAX));
           PingReplyShort();
           done = true;
        }
        // <#KEYPR=x>
        if (!done && MsgInBuff.substring(BC_MSG_START,BC_MSG_START+BC_MSG_LEN) == KEY_PRESSED &&
           MsgInBuff.charAt(BC_MSG_DATA) == '=') {
             
           //SmartKeys = MsgInBuff.substring(BC_MSG_DATA+1);  
           done = true;
        }
        // <#SHOW1=text>
        if (!done && MsgInBuff.substring(BC_MSG_START,BC_MSG_START+BC_MSG_LEN) == SHOW_ROW_1 &&
           MsgInBuff.charAt(BC_MSG_DATA) == '=') {
           done = true;
        }
        // <#SHOW2=text>
        if (!done && MsgInBuff.substring(BC_MSG_START,BC_MSG_START+BC_MSG_LEN) == SHOW_ROW_2 &&
           MsgInBuff.charAt(BC_MSG_DATA) == '=') {
           done = true;
        }
       
       // <#TIME_=13:03:00> 
       if (!done && MsgInBuff.substring(BC_MSG_START,BC_MSG_START+BC_MSG_LEN) == TIME_BROADCAST &&
          MsgInBuff.charAt(BC_MSG_DATA) == '=') {
          s = MsgInBuff.substring(BC_MSG_DATA+1,BC_MSG_DATA+3);
          MyTime.hour = s.toInt();
          s = MsgInBuff.substring(BC_MSG_DATA+4,BC_MSG_DATA+6);
          MyTime.minute = s.toInt();
          s = MsgInBuff.substring(BC_MSG_DATA+7,BC_MSG_DATA+9);
          MyTime.second = s.toInt();
          //PingReplyLong();
          done = true;
      }

      #ifdef I_DO_RELAYS
      if (!done && MsgInBuff.charAt(BC_MSG_START)==SM_RELAY_CMND){
         i = 0; done = false;
         while (!done){
            s = menu_table[i].cmnd;
            //Serial.println(s); Serial.println(i);
            if (MsgInBuff.substring(BC_MSG_START,BC_MSG_START+BC_MSG_LEN) == s ){
               done = true;
            } 
            else {
              if( ++i >= MAX_MENU_INDX) done = true;
            }
         }

         if ((MsgInBuff.charAt(BC_MSG_DATA)==SM_ASSIGN) ||
             (menu_table[i].ru == Me.index ) ||
             (MsgInBuff.charAt(BC_MSG_DATA+1)==SM_ON) || 
             (MsgInBuff.charAt(BC_MSG_DATA+1)==SM_OFF) ||
             (MsgInBuff.charAt(BC_MSG_DATA+1)==SM_TOGGLE)){
                //RelayControl(menu_table[i].ri,MsgInBuff.charAt(BC_MSG_DATA+1)); 
                //Serial.print("Relay "); Serial.print(menu_table[i].ri); Serial.print("-");
                //Serial.println(MsgInBuff.charAt(BC_MSG_DATA+1)); 

         }
         
         
      }
      #endif
   }
}
*/
