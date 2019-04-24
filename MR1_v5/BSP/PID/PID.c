#include "main.h"


// 初始化PID参数
void PID_parameter_init(PID *pp, float Kp, float Ki, float Kd, float outputmax, float Integralmax)  
{  
		pp->Integralmax = Integralmax;
	  pp->outputmax = outputmax;
	  pp->Proportion = Kp;
	  pp->Integral   = Ki;
	  pp->Derivative = Kd;
    pp->DError = pp->Error = pp->output = pp->LastError = pp->PrevError = pp->errormax = 0.0f; 
}  


// 清除pid里的error等参数
void PID_clear_error(PID *pp)
{
    pp->DError = pp->Error = pp->output = pp->LastError = pp->PrevError = pp->errormax = 0.0f; 	
}


// 对变量进行范围限制
float PID_abs_limit(float a, float ABS_MAX)
{
    if(a > ABS_MAX)
        a = ABS_MAX;
		
    if(a < -ABS_MAX)
        a = -ABS_MAX;
		return a;
}

// 增量式PID
void PID_incremental_PID_calculation(PID *pp,  float  CurrentPoint, float NextPoint)  
{  
        static u8 flag = 1;
	
				pp->Error =  NextPoint -  CurrentPoint;

				if(flag)
				{
					pp->LastError = pp->Error;
					flag = 0;
				}
				
        pp->DError = pp->Error - pp->LastError;
				
        pp->output +=  pp->Proportion * ( pp->Error - pp->LastError )+   \
											 PID_abs_limit(pp->Integral * pp->Error, pp->Integralmax ) +   \
											 pp->Derivative * ( pp->Error +  pp->PrevError - 2*pp->LastError);  
	
	      if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	      if(pp->output < - pp->outputmax )  
					pp->output = -pp->outputmax;
	      pp->PrevError = pp->LastError;  
        pp->LastError = pp->Error;
}


// 增量式PID,直接传入误差
void PID_incremental_PID_calculation_by_error(PID *pp,  float error)
{  
        static u8 flag = 1;
	
				pp->Error =  error;

				if(flag)
				{
					pp->LastError = pp->Error;
					pp->PrevError = pp->LastError;
					flag = 0;
				}
				
				pp->DError = pp->Error - pp->LastError;
				
        pp->output +=  pp->Proportion * ( pp->Error - pp->LastError )+   \
											 pp->Integral * pp->Error +  \
											 pp->Derivative * ( pp->Error +  pp->PrevError - 2*pp->LastError);  
	
	      if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	      if(pp->output < - pp->outputmax )  
					pp->output = -pp->outputmax;
	      pp->PrevError = pp->LastError;  
        pp->LastError = pp->Error;
}


// 位置式PID
void PID_position_PID_calculation(PID *pp, float CurrentPoint, float NextPoint)  
{   
        pp->Error =  NextPoint -  CurrentPoint;          
        pp->SumError += pp->Error;                      
        pp->DError = pp->Error - pp->LastError;
	
        pp->output =  pp->Proportion * pp->Error +   \
											PID_abs_limit(pp->Integral * pp->SumError, pp->Integralmax ) +   \
											pp->Derivative * pp->DError ;  

	      if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	      if(pp->output < - pp->outputmax )  pp->output = -pp->outputmax;
//	      pp->PrevError = pp->LastError;  
        pp->LastError = pp->Error;
}

// 位置式PID,直接传入误差
void PID_position_PID_calculation_by_error(PID *pp, float error)  
{   
        pp->Error =  error;          
        pp->SumError += pp->Error;                      
        pp->DError = pp->Error - pp->LastError;
	
        pp->output =  pp->Proportion * pp->Error +   \
											PID_abs_limit(pp->Integral * pp->SumError, pp->Integralmax ) +   \
											pp->Derivative * pp->DError ;  

	      if(pp->output > pp->outputmax )  pp->output = pp->outputmax;
	      if(pp->output < - pp->outputmax )  pp->output = -pp->outputmax;
//	      pp->PrevError = pp->LastError;  
        pp->LastError = pp->Error;
}
