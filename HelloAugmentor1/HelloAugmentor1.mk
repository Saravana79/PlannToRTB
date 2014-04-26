$(eval $(call library,HelloAugmentor1,HelloAugmentor1.cc,augmentor_base rtb bid_request agent_configuration))

$(eval $(call program,HelloAugRunner,HelloAugmentor1 boost_program_options))

RTBKIT_INTEGRATION_TEST_LINK := \
rtb_router bidding_agent integration_test_utils monitor monitor_service augmentor_ex mock_bid_request mock_adserver


