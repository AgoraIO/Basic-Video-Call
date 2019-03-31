//
//  EncryptionType.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, EncrypType) {
    EncrypTypeXTS128,
    EncrypTypeXTS256
};

@interface EncryptionType : NSObject

+ (NSString *)modeStringWithEncrypType:(EncrypType)type;
+ (NSString *)descriptionWithEncrypType:(EncrypType)type;
+ (NSArray *)encrypTypeArray;
@end
