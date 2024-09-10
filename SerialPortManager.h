//---------------------------------------------------------------------------

#ifndef SerialPortManagerH
#define SerialPortManagerH

#include <memory>
#include "Defines.h"
//---------------------------------------------------------------------------
#define GetSerialManager() Singleton<SerialPortManager>::GetInstance()

/**
 * @brief 시리얼 포트 클래스
 * @details 시리얼 포트 연결과 해제를 담당하며 단 하나의 인스턴스만 생성된다.
 *			인스턴스는 크리티컬 섹션으로 보호된다.
 * @author 유영재, yjyoo@visionsemicon.co.kr
 */
class SerialPort : public TVaComm
{
private:
	/**
	 * @brief 인스턴스의 접근을 제한하는 크리티컬 섹션
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	std::shared_ptr<TCriticalSection> csLock;
	int FRegisteredDeviceCount;
	int FCurrentDeviceIndex;
	bool FOpened;

public:
	SerialPort();
	__fastcall ~SerialPort();

	__property bool IsOpened = { read = FOpened };
	__property int CurrentDeviceIndex = { read = FCurrentDeviceIndex };
	__property int RegisteredDeviceCount = { read = FRegisteredDeviceCount };

	/**
	 * @brief 포트를 연다.
	 * @param int portNo : 포트 번호
	 * @param PORTSETTING portSetting : 포트 설정 정보
	 * @return bool : 포트 열기 성공 여부 리턴
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	bool OpenPort(int portNo, PORTSETTING portSetting);

	/**
	 * @brief 포트를 닫는다.
	 * @return 포트 닫기 성공 여부 리턴
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	bool ClosePort();
	void AddDevice() { FRegisteredDeviceCount++; }
	void IncCurrentDeviceIndex();
	void Lock() { csLock->Acquire(); }

	/**
	 * @brief 크리티컬 섹션에 락을 해제한다.
	 * @return void
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	void Unlock() { csLock->Release(); }
};

/**
 * @brief 시리얼 포트 관리자
 * @details 시리얼 포트를 관리하는 객체는 단 하나만 있으면 되므로
 *			싱글톤 템플릿을 상속받아 구현한다.
 * @author 유영재, yjyoo@visionsemicon.co.kr
 */
class SerialPortManager : public Singleton<SerialPortManager>
{
private:
	/**
	 * @brief 포트 리스트
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	TStringList *PortList;

	/**
	 * @brief 포트를 생성한다.
	 * @details 신규 포트를 생성하여 리스트에 추가한다.
	 * @param int portNo : 포트 번호
	 * @return SerialPort* : 생성한 포트의 포인터
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* AddPort(int portNo);

	/**
	 * @brief 포트를 제거한다.
	 * @details 포트를 리스트에서 삭제한 후 인스턴스를 제거한다.
	 * @param int portNo : 포트 번호
	 * @return SerialPort* : 삭제한 포트의 포인터
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* DeletePort(int portNo);

	/**
	 * @brief 포트를 찾는다.
	 * @details 포트 리스트에 포트 번호에 해당하는 포트 인스턴스가 있는지 검색한다.
	 * @param int portNo : 포트 번호
	 * @return SerialPort* : 찾은 포트의 포인터. 없으면 NULL 리턴.
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* FindPort(int portNo);

public:
	/**
	 * @brief 생성자
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	SerialPortManager();

	/**
	 * @brief 소멸자
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	~SerialPortManager();

	/**
	 * @brief 해당 설정의 포트에 대한 포인터를 리턴한다.
	 * @details 해당 포트를 검색하여 있으면 바로 포인터를 리턴하고, 없으면 새로 생성 후 리턴한다.
	 * @param int portNo : 포트 번호
	 * @param PORTSETTING portSetting : 포트 설정 정보
	 * @return SerialPort* : 포트의 포인터.
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* GetPort(int portNo, PORTSETTING portSetting);

	/**
	 * @brief 해당 포트를 제거한다.
	 * @param int portNo : 포트 번호
	 * @return bool : 삭제 성공 여부 리턴
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	bool RemovePort(int portNo);
};

#endif
