//
//  Settings.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/4.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "Settings.h"

@implementation Settings
- (instancetype)init {
    if (self = [super init]) {
        self.encryption = [[Encryption alloc] init];
    }
    return self;
}
@end
