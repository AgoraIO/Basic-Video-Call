//
//  FileCenter.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2019/5/6.
//  Copyright © 2019 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FileCenter : NSObject
+ (NSString *)logDirectory;
+ (NSString *)logFilePath;
+ (NSString *)audioFilePath;
@end

NS_ASSUME_NONNULL_END
