//
//  EncryptionType.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, EncryptionType) {
    EncryptionTypeXTS128,
    EncryptionTypeXTS256,
    EncryptionTypeNone,
};

@interface Encryption : NSObject
@property (nonatomic, copy, readonly) NSString *modeString;
@property (nonatomic, copy, readonly) NSString *description;
@property (nonatomic, assign) EncryptionType type;
@property (nonatomic, copy) NSString *secret;

+ (NSString *)modeStringWithType:(EncryptionType)type;
+ (NSString *)descriptionWithType:(EncryptionType)type;
+ (NSArray *)allTypesArray;
@end
