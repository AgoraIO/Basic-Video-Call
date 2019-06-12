//
//  AppDelegate.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/11/17.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.orientation = UIInterfaceOrientationMaskPortrait;
    return YES;
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    return self.orientation;
}
@end
