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
        let windowsQuery = app.windows
        let channelNameTextField = windowsQuery.textFields["Channel Name"]
        channelNameTextField.click()
        channelNameTextField.typeText("uiTestChannel")
        
        let joinButton = windowsQuery.buttons["Join"]
        joinButton.click()
        
        let closeButton = windowsQuery.children(matching: .button).element(boundBy: 4)
        expectation(for: NSPredicate(format: "exists == 1"), evaluatedWith: closeButton, handler: nil)
        waitForExpectations(timeout: 5, handler: nil)
        
        closeButton.click()
        
        let textField = windowsQuery.textFields["Channel Name"]
        expectation(for: NSPredicate(format: "exists == 1"), evaluatedWith: textField, handler: nil)
        waitForExpectations(timeout: 5, handler: nil)
    }
}
