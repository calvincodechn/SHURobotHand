#include "drv_timer.h" 
#include "sys_fuction.h"


GPIO_TypeDef* GPIO_TYPE[] =    {GPIOD, GPIOE, GPIOD, GPIOB, GPIOE, GPIOD};
uint16_t GPIO_POSITIVE_PIN[] = {GPIO_Pin_14,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_0,GPIO_Pin_10};
uint16_t GPIO_NEGETIVE_PIN[] = {GPIO_Pin_15, GPIO_Pin_10,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_1,GPIO_Pin_11};

void timer_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM5ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = 1000; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=840;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2//��Ϊͨ�üĴ���������Ϊ10ms

	//TIM_TimeBaseInitStructure.TIM_Period = 1000; 	//�Զ���װ��ֵ
	//TIM_TimeBaseInitStructure.TIM_Prescaler=84;  //��ʱ����Ƶ
	//TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	//TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM5����Ϊͨ�üĴ���������Ϊ1ms

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //������ʱ��2�����ж�
	//TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //������ʱ��5�����ж�

	TIM_Cmd(TIM2,ENABLE); 
	//TIM_Cmd(TIM5,ENABLE); 

	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��5�ж�
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //��ռ���ȼ�3
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�2
	//NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}


void timer_pwm_init(uint16_t timer_prescaler, uint32_t timer_period)
{		 					 

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTFʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOC6����Ϊ��ʱ��3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);          
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;        
	GPIO_Init(GPIOD,&GPIO_InitStructure);          
	  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=12;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=467;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3

	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
	
	  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_Pulse=0;
    
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
	
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	
	TIM3->CCR3 = 234;
	
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM 
	TIM_Cmd(TIM4, ENABLE); 
}	
	


void gpio_motor_dir_init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}


void timer_pwm_fresh(FINGEL_SELECT f_select, uint16_t pwmval)
{
	switch (f_select)
	{
		case FINGER_SEL_THUMB:
			TIM_SetCompare1(TIM4,pwmval);//need to be changed
			break;
		case FINGER_SEL_INDEX:
			TIM_SetCompare1(TIM3,pwmval);
			break;
		case FINGER_SEL_MIDDLE:
			TIM_SetCompare2(TIM3,pwmval);
			break;
		case FINGER_SEL_RING:
			TIM_SetCompare3(TIM3,pwmval);
			break;
		case FINGER_SEL_LITTLE:
			TIM_SetCompare2(TIM4,pwmval);
			break;
		case FINGER_SEL_ROOT:
			TIM_SetCompare4(TIM3,pwmval);
			break;
		default:
			break;
	}
}


void set_motor_direction(MOTOR_SERVO_SELECT f_select, SERVO_DIR_SEL servo_dir)
{ 
    switch (servo_dir)
	{
		case SERVO_DIR_UP:
		{
			GPIO_SetBits(GPIO_TYPE[f_select], GPIO_POSITIVE_PIN[f_select]);
			GPIO_ResetBits(GPIO_TYPE[f_select], GPIO_NEGETIVE_PIN[f_select]);
		}
		break;
		case SERVO_DIR_DOWN:
		{
			GPIO_ResetBits(GPIO_TYPE[f_select], GPIO_POSITIVE_PIN[f_select]);
			GPIO_SetBits(GPIO_TYPE[f_select], GPIO_NEGETIVE_PIN[f_select]);
		}
		break;
		case SERVO_DIR_STOP:
		{
			GPIO_ResetBits(GPIO_TYPE[f_select], GPIO_POSITIVE_PIN[f_select]);
			GPIO_ResetBits(GPIO_TYPE[f_select], GPIO_NEGETIVE_PIN[f_select]);
		}
		break;
		default:
		break;
	}
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)  
	{ 
		//static unsigned long hand_frequency = 0;
		//if(hand_frequency > 19)
		//{
			//Current_Protect();
			//hand_frequency=0;
		//}
		//hand_frequency=hand_frequency+1;
		//TIM_SetCompare3(TIM3, );
		//sys_main_start=0x01;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
   }

}

 

 