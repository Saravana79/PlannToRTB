$(eval $(call program,HelloRTBKit-1,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-1-S,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-1-V,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-1-B,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-1-S-B,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-1-R,bidding_agent rtb_router boost_program_options services))
$(eval $(call program,HelloRTBKit-1-S-R,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-3,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-3-S,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-5-S,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-5,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-10,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-11,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-12,bidding_agent rtb_router boost_program_options services))
## $(eval $(call program,HelloRTBKit-14,bidding_agent rtb_router boost_program_options services))
RTBKIT_INTEGRATION_TEST_LINK := \
        rtb_router bidding_agent integration_test_utils monitor monitor_service augmentor_ex adserver_connector mock_bid_request mock_adserver

## $(eval $(call test,$(RTBKIT_INTEGRATION_TEST_LINK),boost))

## $(eval $(call include_sub_make,nodeagents))

