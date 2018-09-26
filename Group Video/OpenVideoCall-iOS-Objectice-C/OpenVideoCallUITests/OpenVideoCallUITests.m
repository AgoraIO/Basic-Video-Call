//
//  OpenVideoCallUITests.m
//  OpenVideoCallUITests
//
//  Created by GongYuhua on 2017/1/13.
//  Copyright © 2017年 Agora. All rights reserved.
//

#import <XCTest/XCTest.h>

@interface OpenVideoCallUITests : XCTestCase

@end

@implementation OpenVideoCallUITests

- (void)setUp {
    [super setUp];
    
    self.continueAfterFailure = NO;
    [[[XCUIApplication alloc] init] launch];
}

- (void)tearDown {
    [super tearDown];
}

- (void)testJoinAndLeaveChannel {
    XCUIApplication *app = [[XCUIApplication alloc] init];
    XCUIElement *inputNameOfLiveTextField = app.textFields[@"Channel name"];
    [inputNameOfLiveTextField tap];
    [inputNameOfLiveTextField typeText:@"uiTestChannel"];
    [app.buttons[@"JoinChannel"] tap];
    
    XCUIElement *closeButton = app.buttons[@"btn endcall"];
    [self expectationForPredicate:[NSPredicate predicateWithFormat:@"exists == 1"] evaluatedWithObject:closeButton handler:nil];
    [self waitForExpectationsWithTimeout:5 handler:nil];
    
    [closeButton tap];
    
    XCUIElement *textField = app.textFields[@"Channel name"];
    [self expectationForPredicate:[NSPredicate predicateWithFormat:@"exists == 1"] evaluatedWithObject:textField handler:nil];
    [self waitForExpectationsWithTimeout:5 handler:nil];
}

@end
