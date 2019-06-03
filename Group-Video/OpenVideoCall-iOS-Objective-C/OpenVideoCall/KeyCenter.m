//
//  KeyCenter.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "KeyCenter.h"

@implementation KeyCenter
+ (NSString *)AppId {
    return @"3d76392019ca47599548a67ad27c9699";
}

// you can specify a token here to test when you have enabled your app certificate
// a temp token can be generated from your project dashboard for testing purpose. Long-term wise
// it's suggested to deploy your own token server
+ (NSString *)Token {
    return @"0063d76392019ca47599548a67ad27c9699IABp7HTvR7acbVvihFYEMCKyD/3MavmTM73WUXbc2ZEILwx+f9gAAAAAEAD4QVCVyUr2XAEAAQDISvZc";
}
@end
