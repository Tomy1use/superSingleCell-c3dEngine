//
//  c3dDeviceAndOSInfo.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-4-18.
//
//

#include "c3dDeviceAndOSInfo.h"
static Cc3dDeviceAndOSInfo*s_deviceAndOSInfo=NULL;
Cc3dDeviceAndOSInfo*Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo(){
    if(s_deviceAndOSInfo==NULL){
        s_deviceAndOSInfo=new Cc3dDeviceAndOSInfo();
    }
    return s_deviceAndOSInfo;
}

Cc3dVector2 Cc3dDeviceAndOSInfo::getScreenSize(){
    CGRect rect=[[UIScreen mainScreen] bounds];
    CGSize size=rect.size;
    CGFloat width=size.width;
    CGFloat height=size.height;
    return Cc3dVector2(width,height);
}
Cc3dVector2 Cc3dDeviceAndOSInfo::getResolutionSize(){
    CGRect rect=[[UIScreen mainScreen] bounds];
    CGSize size=rect.size;
    CGFloat width=size.width;
    CGFloat height=size.height;
    CGFloat scale_screen=[UIScreen mainScreen].scale;
    CGFloat widthReso=width*scale_screen;
    CGFloat heightReso=height*scale_screen;
    return Cc3dVector2(widthReso,heightReso);
}
void Cc3dDeviceAndOSInfo::getMemStatistics(int&memUsed,int&memFree,int&memTotal){//获得内存统计数据
    //引自：http://stackoverflow.com/questions/5012886/knowing-available-ram-on-an-ios-device
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);
    //
    vm_statistics_data_t vm_stat;
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS){
        cout<<"Failed to fetch vm statistics"<<endl;
    }
    // Stats in bytes
    natural_t mem_used = (vm_stat.active_count +
                          vm_stat.inactive_count +
                          vm_stat.wire_count) * pagesize;
    natural_t mem_free = vm_stat.free_count * pagesize;
    natural_t mem_total = mem_used + mem_free;
    memUsed=mem_used;
    memFree=mem_free;
    memTotal=mem_total;
    
}
int Cc3dDeviceAndOSInfo::getSupportedMaxTextureSize(){
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return maxTextureSize;
}
