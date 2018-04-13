/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "SmartDesk/DeskRequester.hpp"

DeskRequester::DeskRequester() {
    Init();
}

void DeskRequester::Init() {
    mReq.Init(DESK, this);
    mReq.bind();
    mReq.start();
}

void DeskRequester::Run() {
    printf("Select a command\n");
    printf("[1]: Move Up\n");
    printf("[2]: Move Down\n");
    printf("[3]: Stop\n");
    printf("[4]: Get Height\n");
    printf("[5]: Set Height\n");
    printf("[6]: Get Data\n");
    printf("[7]: Set Data\n");
    printf("[8]: get UUID\n");
    printf("[9]:  Save To Pos1\n");
    printf("[10]: Move To Pos1\n");
    printf("[11]: Save To Pos2\n");
    printf("[12]: Move To Pos2\n");
    printf("[13]: Save To Pos3\n");
    printf("[14]: Move To Pos3\n");
    printf("[15]: Save To Pos4\n");
    printf("[16]: Move To Pos4\n");
    printf("[17]: Dual Motor, Single Leg Reset\n");
    printf("[18]: Get Controller Status\n");
    printf("[19]: Turn On Speaker\n");
    printf("[20]: Turn Off Speaker\n");
    printf("[21]: Mic Led On\n");
    printf("[22]: Mic Led Off\n");
    printf("[23]: Single Motor Dual Leg Reset\n");
    printf("[24]: Rpi Backlight On\n");
    printf("[25]: Rpi Backlight Off\n");
    printf("[26]: Rpi set brightness\n");
    printf("[27]: Exit\n");
    printf("Select: ");
    scanf("%s",s);
    printf("input= %s\n",s);
    a = atoi(s);
    switch(a){
        case 1: 
            // move up
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.Up; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x0F; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            mReq.putBytes(tx,len);                
            printf("Set Desk Move Up\n");                
            printf("Sending Data to Desk...\n");
            break;
        case 2: //Move Down
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.Down; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x10; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            mReq.putBytes(tx,len); 
            printf("Set Desk Move Down\n");
            printf("Sending Data to Desk...\n");
            break;
        case 3: // Stop
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.Stop; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x11; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            mReq.putBytes(tx,len); 
            printf("Set Desk Stop\n");  
            printf("Sending Data to Desk...\n");
            break;
        case 4: // get height
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Read; // instruction
            tx[6] = ControlTableAddress.Height; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x0D; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            mReq.putBytes(tx,len); 
            printf("Get Height of Desk\n");
            printf("Sending Data to Desk...\n");
            break;
        case 5: // Set Height
            printf("height = :");
            char str1[10];
            scanf("%s",str1);
            printf("\n");
            h = atoi(str1);
            printf("h=%d\n",h);
            b = (char)((h >> 8)&0x00ff);
            printf("b = %x\n",b);
            c = (char)(h&0x00ff);
            printf("c = %x\n",c);
            tx[0] = 0xFF;
            tx[1] = 0xFF;
            tx[2] = 0x02; //length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.SetPointHeight; // Addr
            tx[7] = b; // data1
            tx[8] = c; // data2
            sum = 10;
            sum += tx[6];
            sum += (short)c+(short)b;
            if(sum > 255) sum =sum%255;
            tx[9] = (char)sum;
            tx[10] = 0xFA; 
            tx[11]= 0xFA;
            len = 12;
            for(int i = 0; i < len; i++){
                printf("%x ",tx[i]);
            }
            printf("\n");
            mReq.putBytes(tx,len); 
            printf("Sending Data to Desk...\n");
            break;
        case 6: // get data
            printf("[1]: Version \n");
            printf("[2]: Current Height \n");
            printf("[3]: Up \n");
            printf("[4]: Down \n");
            printf("[5]: Stop \n");
            printf("[16]: Min Height \n");
            printf("[17]: Max Height \n");
            printf("[18]: Height Setpoint \n");
            printf("[19]: Operation Mode \n");
            printf("[20]: Process ID \n");
            printf("[21]: Box Status \n");
            printf("[22]: State \n");
            printf("[23]: Direction\n ");
            printf("[24]: Position 1 Height\n ");
            printf("[25]: Position 2 Height\n ");
            printf("[26]: Position 3 Height\n ");
            printf("[27]: Position 4 Height\n ");
            printf("Address = :");
            scanf("%s",str);
            printf("\n");
            addr = atoi(str);
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = 0x03; //product id
            tx[4] = 0x03; // component id
            tx[5] = 0x01; // instruction
            tx[6] = addr; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            mReq.putBytes(tx,len); 
            printf("Get Data From Desk\n");
            printf("Sending Data to Desk...\n");
            break;
        case 7: //set data
            printf("[1]: Version \n");
            printf("[2]: Current Height \n");
            printf("[3]: Up \n");
            printf("[4]: Down \n");
            printf("[5]: Stop \n");
            printf("[16]: Min Height \n");
            printf("[17]: Max Height \n");
            printf("[18]: Height Setpoint \n");
            printf("[19]: Operation Mode \n");
            printf("[20]: UUID \n");
            printf("[21]: Box Status \n");
            printf("[22]: Box State \n");
            printf("Address = :");
            scanf("%s",str);
            printf("\n");
            addr = atoi(str);
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = 0x03; //product id
            tx[4] = 0x03; // component id
            tx[5] = 0x02; // instruction
            tx[6] = addr; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            printf("Value = :");
            scanf("%s",str);
            printf("\n");
            h = atoi(str);
            printf("h=%d\n",h);
            b = (char)((h >> 8)&0x00ff);
            printf("b = %x\n",b);
            c = (char)(h&0x00ff);
            printf("c = %x\n",c);
            tx[7] = b;
            tx[8] = c;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            mReq.putBytes(tx,len); 
            printf("Set Data to Desk\n");
            printf("Sending Data to Desk...\n");
            break;
        case 8: // get UID
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Read; // instruction
            tx[6] = ControlTableAddress.UserID; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            mReq.putBytes(tx,len); 
            printf("Get UUID of Desk\n");
            printf("Sending Data to Desk...\n");
            break;
        case 9: // Save to Pos 1
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.SaveToPos1; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            mReq.putBytes(tx,len); 
            printf("Save Current Height To POS1\n");                
            printf("Sending Data to Desk...\n");
            break;
        case 10: // Move to pos 1
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.MoveToPos1; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            mReq.putBytes(tx,len); 
            printf("Move To POS1\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 11: // Save to pos 2
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.SaveToPos2; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Save Current Height To POS2\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 12: // Move to pos 2
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.MoveToPos2; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Move To POS2\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 13: //Save to position 3
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.SaveToPos3; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Save Current Height To POS3\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 14: //move to position 3
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.MoveToPos3; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Move To POS3\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 15: //save to position 4
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.SaveToPos4; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Save Current Height To POS3\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 16: // move to position 4
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.MoveToPos4; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Move To POS4\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 17: // Dual Motor Reset
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction
            tx[6] = ControlTableAddress.DualMotorReset; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Reset\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 18: // Get Controller Status
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Read; // instruction write
            tx[6] = ControlTableAddress.BoxStatus; // address
            tx[7] = 0x01; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Get Controller Status\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");
            break;
        case 19: // Turn On Speaker
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = ControlTableAddress.Speaker; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Get Controller Status\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");                
            break;
        case 20:// Turn Off Speaker
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = ControlTableAddress.Speaker; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x00; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Turn Off Speake\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");                
            break;
        case 21: // Turn on Mic Leds
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = ControlTableAddress.Mic; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Turn Off Speake\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");                
            break;
        case 22: // Turn Off Mic Leds
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = ControlTableAddress.Mic; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x00; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Turn Off Mic Leds\n");
            //mZmq.mRequester.sendOne();
            printf("Sending Data to Desk...\n");                
            break;
        case 23: // Single Motor Reset
            
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.UpDownMotor; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = ControlTableAddress.SingleMotorReset; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x00; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len); 
            printf("Doing Reset\n");
            //mZmq.mRequester.sendOne();    
            break;
        case 24: // RPI backlight on
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.RpiBoard; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = RpiControlTable.Backlight; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x00; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len);    
            break;
        case 25: // RPI backlight Off
            tx[0] = 0xFF; // Start Byte 1
            tx[1] = 0xFF; // Start Byte 2
            tx[2] = 0x02; // length
            tx[3] = ProductId.SmartDesk; //product id
            tx[4] = ComponentId.RpiBoard; // component id
            tx[5] = Instructions.Write; // instruction write
            tx[6] = RpiControlTable.Backlight; // address
            tx[7] = 0x00; // param 1
            tx[8] = 0x01; // param 2
            tx[9] = 0x00; // check sum
            tx[10] = 0xFA;// stop byte 1
            tx[11] = 0xFA;// stop byte 2
            len = 12;
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum = sum%255;
            tx[9] = (unsigned char)sum;
            //mZmq.mRequester.addTxData(tx,len);
            mReq.putBytes(tx,len);
            break;
        case 26: // RPI set brightness
            printf("brightness percent (0-100) = :");
            char str[10];
            scanf("%s",str);
            printf("\n");
            h = atoi(str);
            printf("h=%d\n",h);
            b = (char)((h >> 8)&0x00ff);
            printf("b = %x\n",b);
            c = (char)(h&0x00ff);
            printf("c = %x\n",c);
            tx[0] = 0xFF;
            tx[1] = 0xFF;
            tx[2] = 0x02; //length
            tx[3] = ProductId.SmartDesk; // Product
            tx[4] = ComponentId.RpiBoard; // Coponent
            tx[5] = Instructions.Write; // Instruction
            tx[6] = RpiControlTable.Brightness; // Addr
            tx[7] = b; // data1
            tx[8] = c; // data2
            sum = tx[2]+tx[3]+tx[4]+tx[5]+tx[6]+tx[7]+tx[8];
            if(sum > 255) sum =sum%255;
            tx[9] = (char)sum;
            tx[10] = 0xFA; 
            tx[11]= 0xFA;
            len = 12;
            mReq.putBytes(tx,len); 
            break;
        case 27: 
//          mZmq.mRequester.sendNull();
            exit(EXIT_SUCCESS);
            break;
        default:
//                mZmq.mRequester.sendNull();
            break;
    }
    for(int i = 0; i < len; i++) printf("%x ",tx[i]);
    printf("\n");
    a = 0;
    memset(s,0,sizeof(s));

}

DeskRequester::~DeskRequester() {

}
int ZmqReqCallback(void *local_class, void *target_class){
    ZmqRequester *x = (ZmqRequester *)local_class;

    AutonomousCommFrame_Typedef obj;
    
    uint16_t data = AutonomousCommFrame_GetParamsFromArray(x->rx_buf,x->rx_buf_length);
    printf("Get data from %s, data: %d\n",DESK,data);
    return 0;
  
}



