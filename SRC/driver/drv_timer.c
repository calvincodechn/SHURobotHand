#include "drv_timer.h" 
//#include "sys_fuction.h"


GPIO_TypeDef* GPIO_TYPE[] =    {GPIOD, GPIOE, GPIOD, GPIOB, GPIOE, GPIOD};
uint16_t GPIO_POSITIVE_PIN[] = {GPIO_Pin_14,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_0,GPIO_Pin_10};
uint16_t GPIO_NEGETIVE_PIN[] = {GPIO_Pin_15, GPIO_Pin_10,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_1,GPIO_Pin_11};
static current_callback current_callback_func = NULL;
 
static pressure_callback pressure_callback_func = NULL;

static angle_callback angle_callback_func = NULL;

static uint16_t time_count = 0;

void timer_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  

	TIM_TimeBaseInitStructure.TIM_Period = 999; 	//(arr)
	TIM_TimeBaseInitStructure.TIM_Prescaler=13;   //(psc)  Tclk = 84Mhz  // Tout = ((arr+1)*(psc+1))/Tclk
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //Count up mode
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//INIT TIM2 //The cycle is 166us

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //Allow interrupts
	TIM_Cmd(TIM2,ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //Preemption priority 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //Sub priority 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure); 
}

void timer_pwm_init(uint16_t timer_prescaler, uint32_t timer_period)
{				 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTF时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOC6复用为定时器3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);          

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;        
	GPIO_Init(GPIOD,&GPIO_InitStructure);          
 
	TIM_TimeBaseStructure.TIM_Prescaler=12;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=467;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse=0;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 

	TIM3->CCR3 = 234;

	TIM_Cmd(TIM3, ENABLE);  //使能TIM 
	TIM_Cmd(TIM4, ENABLE); 
}	

void gpio_motor_dir_init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
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
void current_sys_callback(current_callback current_callback_in)
{
  current_callback_func = current_callback_in;
}

void pressure_sys_callback(pressure_callback pressure_callback_in)
{
  pressure_callback_func = pressure_callback_in;
}

void angle_sys_callback(angle_callback angle_callback_in)
{
  angle_callback_func = angle_callback_in;
}



void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)  //per 1/6 ms
  {
    if(current_callback_func)
    {
      current_callback_func();
    }
    if(time_count == 300)  //per 50ms Collection angle
    {
      time_count = 0;
      if(angle_callback_func)
        angle_callback_func();  
	  if(pressure_callback_func)
        pressure_callback_func();
	}
    else 
      time_count++;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  }
}
