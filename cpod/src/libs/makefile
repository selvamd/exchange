##
## Libs makefile
##

include ../make.include

#Include directories
CCINCLUDE = -I. -I$(GLOBAL_INCLUDE) -I$(GENERATED_CODE_INCLUDE) $(EXT_LIBS_INCLUDE) -I$(LIBS_INCLUDE) -I$(MYSQL_INCLUDE_DIR)

#Compilation flags 
CPPFLAGS = $(CCINCLUDE) -D_REENTRANT -std=c++11 -g

LIB = exchange.a
TEMP_LIB = temp_exchange.a

TEST_PROGS = accept_server \
			 accept_client \
			 xml_stream_handler_test \
			 xml_stream_handler_test_client \
			 multicast_test_publisher \
			 multicast_test_reader

UNIT_TEST_PROGS = SelectSocketMultiplexer_ut \
				  InternalApiSession_ut \
				  MsgBuffer_ut \
				  ExchangeApiMsgBuffer_ut \
				  DBTest_ut \
				  Book_ut \
				  SessionCollection_ut \
				  ApplicationXmlConfigurationParser_ut \
				  ApplicationEnvironment_ut \
				  ConfigurationGenerator_ut \
				  ConnectionConfigParser_ut \
				  Logger_ut \
				  XmlStreamHandler_ut \
				  XmlSslStreamHandler_ut \
				  ApiStreamHandler_ut \
				  AnyStreamHandler_ut \
				  SslStreamHandler_ut \
				  WebSocketStreamHandler_ut \
				  XmlApiAdapter_ut \
				  JsonApiAdapter_ut \
				  TradingAccountDomainObject_ut \
				  InsAndSortVector_ut \
				  HttpStreamHandler_ut \
				  WebServerSessionFactory_ut \
				  LogFormatter_ut \
				  XmlTree_ut \
				  MetricsTracker_ut \
				  ExchangePersistentDataManager_ut \
				  MySqlUtils_ut \
				  MysqlDataModel_ut \
				  MysqlDataModel_2_ut \
				  MysqlDataModel_3_ut \
				  MysqlDataModel_4_ut \
				  AppGenericField_ut \
				  MysqlGenericField_ut \
				  MysqlGenericTable_ut \
				  MysqlGenericDatabase_ut \
				  AccountRiskCalculator_ut \
				  AccountStatusCalculator_ut \
				  XmlDataTableParser_ut \
				  XmlTableToRamDbAdapter_ut \
				  CsvParser_ut \
				  DateTimeUtil_ut \
				  XmlRefDataParser_ut \
				  SystemModel_ut \
				  XmlDatabaseParser_ut \
				  FmvTable_ut \
				  XigniteDataModel_ut \
				  ExchDataMgrProxy_ut \
				  XmlApiErrorTranslator_ut \
				  SystemStateDriver_ut \
				  GlobalSystemState_ut \
				  UserSecurityModel_ut \
				  GlobalUtils_ut \
				  XmlNavigator_ut \
				  IndexBivCalculator_ut \
				  IndexCompositionReverseIndex_ut \
				  SymbolMasterListLoader_ut \
				  SymbolMasterList_ut \
				  MysqlDbDataManager_ut \
				  TradeConfirmationNotifier_ut \
				  AppGenericFieldInt32_ut \
				  AppGenericFieldUInt64_ut \
				  AppGenericFieldBoolean_ut \
				  AppGenericFieldInt64_ut \
				  AppGenericFieldDecimal_ut \
				  AppGenericFieldUInt32_ut \
				  AppGenericFieldDate_ut \
				  AppGenericFieldString_ut \
				  AppGenericFieldTimestamp_ut \
				  MysqlGenericFieldUInt64_ut \
				  MysqlGenericFieldBoolean_ut \
				  MysqlGenericFieldInt64_ut \
				  MysqlGenericFieldDecimal_ut \
				  MysqlGenericFieldUInt32_ut \
				  MysqlLookupTableTranslator_ut \
				  MysqlGenericFieldDate_ut \
				  MysqlGenericFieldString_ut \
				  MysqlGenericFieldTimestamp_ut \
				  MysqlGenericDatabase_2_ut \
				  RamDbTestFixture_ut \
				  SqlDbTestFixture_ut \
				  MaturityDate_ut \
				  MaturityDates_ut \
				  EnumIterator_ut \
				  SecurityGlobalUtils_ut \
				  ApplicationDb_ut \
				  ApplicationDbTableBase_ut \
				  ApplicationDbSqlTable_ut \
				  ApplicationDbCacheableTable_ut \
				  ApplicationDbRamTable_ut \
				  RamDbCursor_ut \
				  RamDatabase_gen_ut \
				  ProcessConfigLoader_ut \
				  AppDatabaseGenerator_ut \
				  MysqlDataModelUtilities_ut \
				  PrivateBondIssuanceFacade_ut \
				  MysqlDataModel_PrivateBondIssuance_ut \
				  PrivateBondIssuance_ut \
				  PrivateBondIssuanceOrder_ut \
				  DutchAuction_ut \
				  RamDatabaseFacade_ut \
				  RamDbQueryManager_ut \
				  PrivateBondIssuanceMatcher_ut \
				  PrivateBondIssuanceClearingEventLog_ut \
				  PrivateBondIssuanceBond_ut \
				  PrivateBondIssuanceScenarios_ut \
				  PrivateBondTradingSystem_ut \
				  CpodTradingSystem_ut \
				  PriceTimeMatcher_ut \
				  BondCalculator_ut \
				  PrivateBondOrder_ut \
				  MysqlDataModel_PrivateBondSecondaryMarket_ut \
				  PrivateBondSecondaryMarketFacade_ut \
				  PrivateBondTrade_ut \
				  RecoveryFacade_ut \
				  PrivateBondTradeClearingEventLog_ut \
				  SessionSequenceSharedMemory_ut \
				  SessionMessageBuffer_ut \
				  FlushableRingBuffer_ut \
				  ExchangeApiRingBuffer_ut \
				  MarketDataFacade_ut \
				  TestResetNotify_ut \
				  SessionSequenceRam_ut \
				  CpodMarketFacade_ut \
				  CpodUnderlying_ut \
				  CpodSeries_ut \
				  CpodInstrumentMarketDataCollection_ut \
				  SimpleMarketDataRegistry_ut \
				  InstrumentDataTracker_ut \
				  TradingActivityLogFacade_ut \
				  FixApiAdapter_ut \
				  DynamicPrivateDataRegistry_ut \
				  Blotter_ut \
				  FirmAccountDataFacade_ut \
				  FirmAccountSummary_ut

SAMPLE_UNIT_TEST_PROGS = PortfolioMarginCalculator_ut FmvPriceCalculator_ut

OBJS =	TcpUtils.o \
		ProductionSentinel.o \
		Deployer.o \
		MarketDataStatsMessageProcessorFactory.o \
		MarketDataStatsMessageProcessor.o \
		MarketDataHandlerMessageProcessorFactory.o \
		MarketDataHandlerMessageProcessor.o \
		MarketDataStatsWorkStream.o \
		MarketDataStatsExecutor.o \
		MarketDataClientMessageProcessor.o \
		MarketDataClientMessageProcessorFactory.o \
		MarketDataStreamMessageProcessor.o \
		MarketDataStreamMessageProcessorFactory.o \
		MarketDataCounterMessageProcessor.o \
		MarketDataCounterMessageProcessorFactory.o \
		RamDbQueryMessageProcessorFactory.o \
		RamDbQueryMessageProcessor.o \
		RamDbQueryManager.o \
		RamDbQueryWorkStream.o \
		AppDatabaseGenerator.o \
		AppDatabaseModel.o \
		AppGenericTableModel.o \
		ConfigurationGenerator.o \
		DisconnectList.o \
		SessionInterface.o \
		OperationsSession.o \
		OperationsSessionFactory.o \
		XmlExternalApiSessionFactory.o \
		SelectSocketMultiplexer.o \
		SystemConfiguration.o \
		XmlStreamHandler.o \
		SslStreamHandler.o \
		XmlSslStreamHandler.o \
		XmlApiAdapter.o \
		GlobalUtils.o \
		XigniteUtils.o \
		ApiUtils.o \
		JsonUtils.o \
		XmlUtils.o \
		ExchangeApiMsgBuffer.o \
		SingleCoreProcess.o \
		Book.o \
		MessageProcessorFactory.o \
		ConnectionFactory.o \
		InternalApiSessionFactory.o \
		InternalApiSession.o \
		ApiStreamHandler.o \
		SessionCollection.o \
		FixExternalApiSession.o \
		FixStreamHandler.o \
		FixExternalApiSessionFactory.o \
		XmlExternalApiSession.o \
		LoadTesterWriterWorkStream.o \
		StopWatch.o \
		ApplicationXmlConfigurationParser.o \
		ApplicationEnvironment.o \
		ProcessStatusPublisherWorkStream.o \
		ReliableMulticastPublisher.o \
		ConnectionConfigParser.o \
		QueryMessageProcessor.o \
		Logger.o \
		MulticastFeedHandler.o \
		CharDataMulticastHandler.o \
		AnyStreamHandler.o \
		SessionCallbackWorkStream.o \
		SessionNotificationWorkStream.o \
		GatewayError.o \
		HttpStreamHandler.o \
		WebServerSession.o \
		WebServerSessionFactory.o \
		SessionFactoryCollection.o \
		OpsWebAgentMessageProcessor.o \
		ExchangeGlobals.o \
		OpsWebAgentMessageProcessorFactory.o \
		LogFormatter.o \
		XmlTree.o \
		MySqlUtils.o \
		MysqlDataModel.o \
		AppGenericFieldDecimal.o \
		MysqlGenericFieldDecimal.o \
		MysqlGenericTable.o \
		MysqlGenericDatabase.o \
		RamDatabase.o \
		OpsHelper.o \
		AccountRiskCalculator.o \
		AccountStatusCalculator.o \
		TrancheNormalCopula.o \
		PortfolioMarginCalculator.o \
		XmlDataTableParser.o \
		ExchangePersistentDataManager.o \
		MysqlToXmlTableAdapter.o \
		ExchangeProcessOpsMessageProcessor.o \
		ExchangeProcessOpsMessageProcessorFactory.o \
		XmlTableToRamDbAdapter.o \
		RefDataToCSV.o \
		RMCAckReceiver.o \
		RMCBuffer.o \
		RMCFeedHandler.o \
		RMCPublisher.o \
		RMCSendBuffer.o \
		RMCSubscriberAckSender.o \
		RMCSubscriber.o \
		RMC_UDPSender.o \
		CsvParser.o \
		OpsMessageProcessorBase.o \
		XmlRefDataParser.o \
		SystemModel.o \
		XmlDatabaseParser.o \
		FmvTable.o \
		FmvPriceCalculator.o \
		BondCalculator.o \
		ZcbIssuanceCalculator.o \
		MysqlToCsvFileAdapter.o \
		XigniteDataModel.o \
		OpsCommandSession.o \
		OpsCommandSessionFactory.o \
		XmlApiErrorTranslator.o \
		GlobalSystemState.o \
		OfflineSystemState.o \
		OpenSystemState.o \
		ClosedSystemState.o \
		SystemStateDriver.o \
		UserSecurityModel.o \
		JobStatusTracker.o \
		CalculateSettlementPrice.o \
		EmailSender.o \
		XmlNavigator.o \
		BivAnalytics.o \
		SecurityGlobalUtils.o \
		EmailMessageProcessor.o \
		NotificationMessageProcessor.o \
		EmailMessageProcessorFactory.o \
		NotificationMessageProcessorFactory.o \
		EmailSessionConnector.o \
		NotificationSessionConnector.o \
		EmailMessageConverter.o \
		PushNotificationStreamHandler.o \
		IndexBivCalculator.o \
		IndexCompositionReverseIndex.o \
		NotificationSender.o \
		TradeConfirmationNotifier.o \
		TradeConfirmationMessageGenerator.o \
		SymbolMasterListLoader.o \
		TradeConfirmationNotificationDb.o \
		MysqlDbDataManager.o \
		MysqlLookupTableTranslator.o \
		SymbolMasterList.o \
		MaturityDate.o \
		MaturityDates.o \
		ApplicationDb.o \
		ApplicationDbTableBase.o \
		ApplicationDbSqlTable.o \
		ApplicationDbCacheableTable.o \
		ApplicationDbRamTable.o \
		MessageRouterInterface.o \
		DutchAuction.o \
		FirmMessageRouter.o \
		PrivateBondIssuanceFacade.o \
		PrivateBondIssuance.o \
		PrivateBondIssuanceOrder.o \
		RamDatabaseFacade.o \
		PrivateBondIssuanceMatcher.o \
		PrivateBondIssuanceTrade.o \
		PrivateBondIssuanceClearingEventLog.o \
		PrivateBondIssuanceBond.o \
		PrivateBondIssuanceBondOwnership.o \
		PrivateBondTradingSystem.o \
		CpodTradingSystem.o \
		PrivateBondOrder.o \
		PrivateBondSecondaryMarketFacade.o \
		PrivateBondTrade.o \
		JsonSession.o \
		JsonSessionFactory.o \
		RecoveryFacade.o \
		PrivateBondTradeClearingEventLog.o \
		SharedMemory.o \
		SharedMemoryMap.o \
		SessionSequenceRam.o \
		SessionSequenceSharedMemory.o \
		SessionMessageBuffer.o \
		FlushableRingBuffer.o \
		ExchangeApiRingBuffer.o \
		RingBufferFlusher.o \
		NullFlusherWorkStream.o \
		MarketDataFacade.o \
		TestResetNotifier.o \
		CpodMarketFacade.o \
		CpodUnderlying.o \
		CpodSeries.o \
		PriceTimeTrade.o \
		CpodOrder.o \
		CpodUserTrade.o \
		CpodOrderArchive.o \
		CpodTradeArchive.o \
		CpodInstrumentMarketDataCollection.o \
		SimpleMarketDataRegistry.o \
		TradingActivityLogFacade.o \
		DynamicPrivateDataRegistry.o \
		FirmAccountDataFacade.o \
		FirmAccountSummary.o

.PHONY : all new profile coverage build_obj build_lib exchange clean install _install

all : $(LIB) install $(TEST_PROGS) $(SAMPLE_UNIT_TEST_PROGS) $(UNIT_TEST_PROGS)

#Coverage flags
coverage : COVFLAGS = -fprofile-arcs -ftest-coverage -O0 -lgcov
coverage : all

#Profile flags
profile : PROFFLAGS = -pg
profile : exchange

#Build exchange only
build_obj : $(TEMP_LIB)
build_lib : $(LIB)
exchange : $(LIB) install
build_uts : $(UNIT_TEST_PROGS)

$(LIB) : $(OBJS)
	- echo -e "$(OK_COLOR)Building static lib........................................................$(NO_COLOR)"
	ar -rcvs $@ $^

$(TEMP_LIB) : $(OBJS)
	- echo -e "$(OK_COLOR)Building object files......................................................$(NO_COLOR)"
	ar -rcvs $@ $^

$(TEST_PROGS) : %:%.cpp $(OBJS) $(LIB)
	$(CXX) $@.cpp $(CPPFLAGS) $(PROFFLAGS) $(COVFLAGS) $(LIB) $(SYSLIBS) $(OPENSSL_LIBS) $(MONGO_DRIVER_LIB) $(MYSQL_LIB) $(POCO_LIBS) -o $@

%.o : %.cpp %.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(PROFFLAGS) $(COVFLAGS) -c $< -o $@

$(UNIT_TEST_PROGS) : %:%.cpp $(OBJS) $(LIB)
	$(CXX) $@.cpp $(CPPFLAGS) $(PROFFLAGS) $(COVFLAGS) $(GTEST_LIB) $(LIB) $(QF_LIB) $(POCO_LIBS) $(SYSLIBS) $(OPENSSL_LIBS) $(MONGO_DRIVER_LIB) $(MYSQL_LIB) $(POCO_LIBS) $(URI_LIB) $(EXCHANGE_LIBS) $(QF_LIB) ../generated_code/libs/generated.a $(EXCHANGE_LIBS) $(SHM_LIBS) $(MONGO_DRIVER_LIB) -o $@

$(SAMPLE_UNIT_TEST_PROGS) : %:%.cpp $(OBJS) $(LIB)
	$(CXX) $@.cpp $(CPPFLAGS) $(PROFFLAGS) $(COVFLAGS) $(GTEST_LIB) $(LIB) $(QF_LIB) $(POCO_LIBS) $(SYSLIBS) $(OPENSSL_LIBS) $(MONGO_DRIVER_LIB) $(MYSQL_LIB) $(POCO_LIBS) $(URI_LIB) $(EXCHANGE_LIBS) ../generated_code/libs/generated.a -o $@

clean : 
	- rm -f *.o core > /dev/null 2>&1
	- rm -f *.a core > /dev/null 2>&1
	- rm -f $(TEST_PROGS)
	- rm -f $(UNIT_TEST_PROGS)
	- rm -f $(SAMPLE_UNIT_TEST_PROGS)
	- rm -f $(LIB_INSTALL_DIR)/$(LIB)
	- rm -f *.gcno
	- rm -f *.gcda
	- rm -f *.gcov
	- rm -f *.junit.xml

install: $(LIB)
	- echo -e "$(OK_COLOR)Installing static lib(s)....................................................$(NO_COLOR)"
	- cp $(LIB) $(LIB_INSTALL_DIR)
	- rm -f temp_*.a core > /dev/null 2>&1

_install:
	- cp $(LIB) $(LIB_INSTALL_DIR)

new :
	$(MAKE) clean
	$(MAKE)

run_ut : $(UNIT_TEST_PROGS)
	for utest in $(UNIT_TEST_PROGS); do ./$$utest; if [ $$? -ne 0 ]; then echo "Error executing: $$utest"; fi; done
run_ut_cov : $(UNIT_TEST_PROGS)
	for utest in $(UNIT_TEST_PROGS); do ./$$utest --gtest_output=xml:$$utest.junit.xml; if [ $$? -ne 0 ]; then echo "Error executing: $$utest"; fi; done
