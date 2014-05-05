#ifndef WGSERVER_WORLDPACKET_H
#define WGSERVER_WORLDPACKET_H

//#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
#include "PublicByteBuffer.h"

//实际发送的消息包//
class WorldPacket : public PublicByteBuffer
{
public:
    __inline WorldPacket() : PublicByteBuffer(), m_opcode(0) { }
    __inline WorldPacket(uint16 opcode, size_t res) : PublicByteBuffer(res), m_opcode(opcode) {}
    __inline WorldPacket(size_t res) : PublicByteBuffer(res), m_opcode(0) { }
    __inline WorldPacket(const WorldPacket &packet) : PublicByteBuffer(packet), m_opcode(packet.m_opcode) {}

    //! Clear packet and set opcode all in one mighty blow
    __inline void Initialize(uint16 opcode )
    {
        clear();
        m_opcode = opcode;
    }

    __inline uint16 GetOpcode() const { return m_opcode; }
    __inline void SetOpcode(const uint16 & opcode) { m_opcode = opcode; }
	//长度放在这里，这么扯淡，要放到开头才行,而且都没有网络字节序的转换,也没有校验值//
    /*__inline void SetLength(const uint16 & len)	{ 
		uint16 * plen = (uint16 * ) &(contents()[2]);
		*plen = len;
	}*/
	__inline void SetLength(const uint16 & len)	{ 
		uint16 * plen = (uint16 * ) &(contents()[0]);
		*plen = len;
	}

	__inline std::string getString()
	{
		//std::string buff = 
		return (const char*)contents();
	}
	
 template<typename T>
   	void SetOffset(const uint16 & offset, const T value ) {
		T * pval = (T *) &(contents()[offset]);
		*pval = value;
	}

public:
	/**
	 * @创建时间 2011-08-31
	 * @创建人 李志勇
	 * @函数作用 向数据包追加字符串
	 * @参数
	 * 	@packet 数据封包指针
	 *  @str 追加的字符串
	 */
	template<typename T> void AppendPacketString(std::string str)
	{
		T ilen = (T)str.size();
		*this << ilen;
		if (ilen > 0)
			Write((const uint8 *) str.c_str(), ilen);
	}
	/**
	 * @创建时间 2011-08-31
	 * @创建人 李志勇
	 * @函数作用 获取字符串
	 * @参数
	 * 	@packet 数据封包
	 * @返回 是否成功
	 */
	template<typename T> bool GetPacketString(std::string &str)
	{
		T ilen;
		*this >> ilen;
		if (ilen == 0)
			return true;
		uint8 *buf = new uint8[ilen+1];
		uint16 plen = size();
		if (plen < ilen)
			return false;
		read(buf, ilen);
		buf[ilen] = 0;
		str = (char*)buf;
		return true;
	}


protected:
    uint16 m_opcode;//一个操作码//
};

/*
template<uint32 Size>
class SERVER_DECL StackWorldPacket : public StackBuffer<Size>
{
	uint16 m_opcode;
public:
	__inline StackWorldPacket(uint16 opcode) : StackBuffer<Size>(), m_opcode(opcode) { }

	//! Clear packet and set opcode all in one mighty blow
	__inline void Initialize(uint16 opcode )
	{
		StackBuffer<Size>::Clear();
		m_opcode = opcode;
	}

	uint16 GetOpcode() { return m_opcode; }
	__inline void SetOpcode(uint16 opcode) { m_opcode = opcode; }
};
*/
//#endif

#endif
