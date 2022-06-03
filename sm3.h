#pragma once

typedef struct{
    unsigned long iplen;        //������Ϣ����
    unsigned long state[8];     //A-H8���Ĵ���
    unsigned char buffer[64];   //�洢������Ϣ�Ļ�����
}sm3_context;

void sm3_starts(sm3_context* ctx);//��ʼ��ctx

void sm3_update(sm3_context* ctx, unsigned char* input, int iplen_t);
/*
 * input    �������е�data
 * iplen_t     data����
 */

void sm3_finish(sm3_context* ctx, unsigned char output[32]);//������ս��

void sm3(unsigned char* input, int iplen_t, unsigned char output[32]);
/*
 * input    �������е�data
 * iplen_t     data����
 * output   ����8���Ĵ����еĽ��
 */