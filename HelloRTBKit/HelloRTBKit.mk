$(eval $(call program,HelloRTBKit-1,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-3,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-5,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-6,bidding_agent rtb_router boost_program_options services))
RTBKIT_INTEGRATION_TEST_LINK := \
        rtb_router bidding_agent integration_test_utils monitor monitor_service augmentor_ex adserver_connector mock_bid_request mock_adserver

## $(eval $(call test,$(RTBKIT_INTEGRATION_TEST_LINK),boost))

## $(eval $(call include_sub_make,nodeagents))

