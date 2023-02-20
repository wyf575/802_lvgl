/*
 * SNManager.h
 *
 *  Created on: 2021年2月1日
 *      Author: Administrator
 */

#ifndef JNI_MODEL_SNMANAGER_H_
#define JNI_MODEL_SNMANAGER_H_

#include "Thread.h"
#include "widgetUI.h"

class SNManager:public Thread{

public:
	static SNManager* getInstance();
	void startGetSnThread();
	void updateQR();
	void initUpdateParam(lv_obj_t* mQRPtr);

protected:
	virtual bool readyToRun();
	virtual bool threadLoop();

private:
	SNManager();

};

#define SNMANAGER	SNManager::getInstance()
#endif /* JNI_MODEL_SNMANAGER_H_ */
