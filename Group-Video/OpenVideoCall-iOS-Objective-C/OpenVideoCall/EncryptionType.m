//
//  EncryptionType.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "EncryptionType.h"

@implementation EncryptionType
+ (NSString *)modeStringWithEncrypType:(EncrypType)type {
    switch (type) {
        case EncrypTypeXTS128: return @"aes-128-xts"; break;
        case EncrypTypeXTS256: return @"aes-256-xts"; break;
        default: return @""; break;
    }
}

+ (NSString *)descriptionWithEncrypType:(EncrypType)type {
    switch (type) {
        case EncrypTypeXTS128: return @"AES 128"; break;
        case EncrypTypeXTS256: return @"AES 256"; break;
        default: return @""; break;
    }
}

+ (NSArray *)encrypTypeArray {
    return @[@(EncrypTypeXTS128), @(EncrypTypeXTS256)];
}
@end
