//
//  OpenVideoCallUITests.swift
//  OpenVideoCallUITests
//
//  Created by GongYuhua on 2017/1/13.
//  Copyright © 2017年 Agora. All rights reserved.
//

import XCTest

class OpenVideoCallUITests: XCTestCase {
        
    override func setUp() {
        super.setUp()
        
        continueAfterFailure = false
        XCUIApplication().launch()
    }
    
    override func tearDown() {
        super.tearDown()
    }
    
    func testJoinAndLeaveChannel() {
        
        let app = XCUIApplication()
        let channelNameTextField = app.textFields["Channel name"]
        channelNameTextField.tap()
        channelNameTextField.typeText("uiTestChannel")
        
        app.buttons["JoinChannel"].tap()
        
        let closeButton = app.buttons["btn endcall"]
        expectation(for: NSPredicate(format: "exists == 1"), evaluatedWith: closeButton, handler: nil)
        waitForExpectations(timeout: 5, handler: nil)
        
        closeButton.tap()
        
        let textField = app.textFields["Channel name"]
        expectation(for: NSPredicate(format: "exists == 1"), evaluatedWith: textField, handler: nil)
        waitForExpectations(timeout: 5, handler: nil)
    }
}
