//
//  EncryptionType.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "Encryption.h"

@implementation Encryption
- (NSString *)modeString {
    switch (_type) {
        case EncryptionTypeXTS128: return @"aes-128-xts"; break;
        case EncryptionTypeXTS256: return @"aes-256-xts"; break;
        case EncryptionTypeNone:   return nil;
    }
}

- (NSString *)description {
    switch (_type) {
        case EncryptionTypeXTS128: return @"AES 128"; break;
        case EncryptionTypeXTS256: return @"AES 256"; break;
        case EncryptionTypeNone:   return nil;
    }
}

+ (NSString *)modeStringWithType:(EncryptionType)type {
    switch (type) {
        case EncryptionTypeXTS128: return @"aes-128-xts"; break;
        case EncryptionTypeXTS256: return @"aes-256-xts"; break;
        case EncryptionTypeNone:   return nil;
    }
}

+ (NSString *)descriptionWithType:(EncryptionType)type {
    switch (type) {
        case EncryptionTypeXTS128: return @"AES 128"; break;
        case EncryptionTypeXTS256: return @"AES 256"; break;
        case EncryptionTypeNone:   return nil;
    }
}

+ (NSArray *)allTypesArray {
    return @[@(EncryptionTypeXTS128), @(EncryptionTypeXTS256)];
}
@end
