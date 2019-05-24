//
//  AppDelegate.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/16.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        UINavigationBar.appearance().barStyle = .blackOpaque
        window?.backgroundColor = UIColor.white
        return true
    }
}
