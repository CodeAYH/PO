//---------------------------------------------------------------------------

#ifndef SerialPortManagerH
#define SerialPortManagerH

#include <memory>
#include "Defines.h"
//---------------------------------------------------------------------------
#define GetSerialManager() Singleton<SerialPortManager>::GetInstance()

/**
 * @brief �ø��� ��Ʈ Ŭ����
 * @details �ø��� ��Ʈ ����� ������ ����ϸ� �� �ϳ��� �ν��Ͻ��� �����ȴ�.
 *			�ν��Ͻ��� ũ��Ƽ�� �������� ��ȣ�ȴ�.
 * @author ������, yjyoo@visionsemicon.co.kr
 */
class SerialPort : public TVaComm
{
private:
	/**
	 * @brief �ν��Ͻ��� ������ �����ϴ� ũ��Ƽ�� ����
	 * @author ������, yjyoo@visionsemicon.co.kr
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
	 * @brief ��Ʈ�� ����.
	 * @param int portNo : ��Ʈ ��ȣ
	 * @param PORTSETTING portSetting : ��Ʈ ���� ����
	 * @return bool : ��Ʈ ���� ���� ���� ����
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	bool OpenPort(int portNo, PORTSETTING portSetting);

	/**
	 * @brief ��Ʈ�� �ݴ´�.
	 * @return ��Ʈ �ݱ� ���� ���� ����
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	bool ClosePort();
	void AddDevice() { FRegisteredDeviceCount++; }
	void IncCurrentDeviceIndex();
	void Lock() { csLock->Acquire(); }

	/**
	 * @brief ũ��Ƽ�� ���ǿ� ���� �����Ѵ�.
	 * @return void
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	void Unlock() { csLock->Release(); }
};

/**
 * @brief �ø��� ��Ʈ ������
 * @details �ø��� ��Ʈ�� �����ϴ� ��ü�� �� �ϳ��� ������ �ǹǷ�
 *			�̱��� ���ø��� ��ӹ޾� �����Ѵ�.
 * @author ������, yjyoo@visionsemicon.co.kr
 */
class SerialPortManager : public Singleton<SerialPortManager>
{
private:
	/**
	 * @brief ��Ʈ ����Ʈ
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	TStringList *PortList;

	/**
	 * @brief ��Ʈ�� �����Ѵ�.
	 * @details �ű� ��Ʈ�� �����Ͽ� ����Ʈ�� �߰��Ѵ�.
	 * @param int portNo : ��Ʈ ��ȣ
	 * @return SerialPort* : ������ ��Ʈ�� ������
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* AddPort(int portNo);

	/**
	 * @brief ��Ʈ�� �����Ѵ�.
	 * @details ��Ʈ�� ����Ʈ���� ������ �� �ν��Ͻ��� �����Ѵ�.
	 * @param int portNo : ��Ʈ ��ȣ
	 * @return SerialPort* : ������ ��Ʈ�� ������
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* DeletePort(int portNo);

	/**
	 * @brief ��Ʈ�� ã�´�.
	 * @details ��Ʈ ����Ʈ�� ��Ʈ ��ȣ�� �ش��ϴ� ��Ʈ �ν��Ͻ��� �ִ��� �˻��Ѵ�.
	 * @param int portNo : ��Ʈ ��ȣ
	 * @return SerialPort* : ã�� ��Ʈ�� ������. ������ NULL ����.
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* FindPort(int portNo);

public:
	/**
	 * @brief ������
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	SerialPortManager();

	/**
	 * @brief �Ҹ���
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	~SerialPortManager();

	/**
	 * @brief �ش� ������ ��Ʈ�� ���� �����͸� �����Ѵ�.
	 * @details �ش� ��Ʈ�� �˻��Ͽ� ������ �ٷ� �����͸� �����ϰ�, ������ ���� ���� �� �����Ѵ�.
	 * @param int portNo : ��Ʈ ��ȣ
	 * @param PORTSETTING portSetting : ��Ʈ ���� ����
	 * @return SerialPort* : ��Ʈ�� ������.
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	SerialPort* GetPort(int portNo, PORTSETTING portSetting);

	/**
	 * @brief �ش� ��Ʈ�� �����Ѵ�.
	 * @param int portNo : ��Ʈ ��ȣ
	 * @return bool : ���� ���� ���� ����
	 * @author ������, yjyoo@visionsemicon.co.kr
	 */
	bool RemovePort(int portNo);
};

#endif
