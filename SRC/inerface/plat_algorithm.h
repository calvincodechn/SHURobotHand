#ifndef _PLAT_ALGORITHM_H
#define _PLAT_ALGORITHM_H

/*  discription: init circular struct list. 
s----------------------------------------------------
    ->  ->      \1  sturct {
    ^    |      \2     data     
	|    V      \3     next
	<-  <-      \4  }plist_name[data_len];
----------------------------------------------------
*******************************************************/
#define init_circular_list(plist_name, data_len) { \
uint32_t data_cur = 0; \
for(data_cur = 0; data_cur < data_len - 1; data_cur++) \
plist_name[data_cur].next = plist_name + data_cur + 1; \
plist_name[data_cur].next = plist_name; \
}


#define init_circular_2dlist(plist_name, data_h_len, data_v_len) { \
uint32_t h_cur = 0, v_cur = 0; \
for(h_cur = 0; h_cur < data_h_len - 1; h_cur++) \
for(v_cur = 0; h_cur < data_v_len - 1; h_cur++) \
{ \
plist_name[h_cur][v_cur].v_next = plist_name[h_cur][v_cur + 1]; \
plist_name[h_cur][v_cur].p_next = plist_name[h_cur + 1][v_cur]; \
} \
for(h_cur = 0; h_cur < data_h_len; h_cur++) \
plist_name[h_cur][data_v_len - 1].v_next = plist_name[h_cur][0]; \
for(v_cur = 0; v_cur < data_v_len; v_cur++) \
plist_name[data_v_len - 1][v_cur].h_next = plist_name[0][v_cur]; \
}

/*  discription: the reading data store in 1xn arry.filter_len stands the
    reading times. buffer_index stand the n-st. refer storing structure below:
    ----------------------------------------------------------------------------
    buffer\data
    buffer_name[0]          -> data[0] data[1] ... data[buffer_index] ...data[n]
    buffer_name[1]          -> data[0] data[1] ... data[buffer_index] ...data[n]
    ...                         ...     ...    ... ...                ......           
    buffer_name[filter_len] -> data[0] data[1] ... data[buffer_index] ...data[n]
    ----------------------------------------------------------------------------
    return_value is median num in buffer_name[0,1,filter_len].data[buffer_index]
*******************************************************************************/
#define filter_buffer(buff_name, filter_len, buff_index, return_value) { \
uint32_t i_cur = 0, j_cur = 0; \
for(i_cur = 0; i_cur < filter_len; i_cur++) { \
for(j_cur = 0; j_cur < filter_len - i_cur; j_cur++) { \
if(buff_name[j_cur].data[buff_index] > buff_name[j_cur + 1].data[buff_index]) {\
uint16_t temp = buff_name[j_cur].data[buff_index]; \
buff_name[j_cur].data[buff_index] = buff_name[j_cur + 1].data[buff_index]; \
buff_name[j_cur + 1].data[buff_index] = temp; \
} \
} \
} \
return_value = ((uint64_t)buff_name[(filter_len - 1) / 2].data[buff_index] + \
(uint64_t)buff_name[(filter_len - 1) / 2 + (filter_len - 1) % 2].data[buff_index]) >> 1; \
}
#endif
