

#ifndef __ACTIVE_PS_PARSER_H_
#define __ACTIVE_PS_PARSER_H_

typedef void APSPARSER_H;  //ACTIVE_PS_PARSER_HANDLE
typedef void APSPARSER_USERDATA;

typedef struct{
	unsigned char es_type;
	unsigned char es_id;
	unsigned char *es_data;
	unsigned int es_data_length;
	unsigned long dts;
	unsigned long pts;
}PES_PACKET_S;

typedef struct{
	unsigned char *scr;
	unsigned char *program_mux_rate;
	unsigned long pack_id;
	PES_PACKET_S *pstPESPacket;
}PS_PACKET_S;

typedef int (*cbDataHandle)(PS_PACKET_S *pstPSPacket, APSPARSER_H * phParser, APSPARSER_USERDATA *pUserData);

#define STREAMID_MAP_SIZE 256


class CTParserBufferManager
{
public:
	CTParserBufferManager();
	~CTParserBufferManager();

public:
	int StartVisitBuffer(unsigned char *pu8PSData, unsigned int u32PSDataLength);  //�������ʻ�����
	int EndVisitBuffer();  //ֹͣ���ʻ�����,δ�������ֽ����뻺������

	int PopParsedBuffer();  //������һ֡�ɹ�,��ѹ���ѽ�����������,�����Լ���¼��ָ��

	unsigned char	*ReadBytes(unsigned int u32Size);
	int				ReadBinary(unsigned int *pu32Out, unsigned int u32Size);
	int				SeekBytes(int s32Size);				// s32Size > 0: ��ǰ, s32Size < 0: ���

private:
	unsigned char * GetOutputBuffer(unsigned int u32NeedBytes);

private:
	unsigned char	*m_pu8Data;
	unsigned int	m_u32DataLength;

private:
	unsigned char	*m_pu8CacheBuffer;
	unsigned int	m_u32CacheBufferSize;
	unsigned int	m_u32CacheDataLength;

private:
	unsigned char   *m_pu8OutputBuffer;
	unsigned int	m_u32OutputBufferSize;

private:
	enum CUR_READ_SEGMENT
	{
		READ_CACHE_SEGMENT,
		READ_CURBUFFER_SEGMENT,
	}m_enReadSegment, m_enPopSegment;
	unsigned char	*m_pu8ReadPos;
	unsigned char	*m_pu8PopPos;
};

class CTActivePSParser : public CTParserBufferManager
{
public:
	CTActivePSParser(cbDataHandle cbDataHandleFuncs, APSPARSER_USERDATA *pUserData);
	~CTActivePSParser();

	int ParsePSData(unsigned char *pu8PSData, unsigned int u32PSDataLength);

private:
	int ParseSystemHeader();
	int ParsePESPacket();

	int WriteInOutputBuffer(unsigned char *pu8Data, unsigned int u32DataLength);

private:
	cbDataHandle m_cbDataHandleFuncs;
	APSPARSER_USERDATA *m_pUserData;

	unsigned char	*m_pu8OutputBuffer;
	unsigned int	m_u32OutputBufferSize;

	unsigned int	m_u32OutputBufferDataLength;
private:
	unsigned char	m_StreamIDMap[STREAMID_MAP_SIZE];
	bool			m_bParseSystemHead;
	unsigned long	m_PacketCounter;

private:
	unsigned char	m_u8MergeID;
	bool			m_bMergeFlag;
};


#endif