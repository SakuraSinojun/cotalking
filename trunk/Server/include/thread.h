/***************************************************
Project: grouptalk
author: TaXueWuHen
base thread;
***************************************************/

#ifndef __TALK_THREAD_H__
#define __TALK_THREAD_H__

class TS_Thread
{
	public:
		TS_Thread();
		~TS_Thread();
		/**
		start thread by start();
		**/
		bool start();
		// stop thread;
		bool stop();
	private:
		pthread_t pid;
		static void *funcThread(void *inst);
	protected:
		bool isRunning;
		virtual void run() = 0;

};


#endif

