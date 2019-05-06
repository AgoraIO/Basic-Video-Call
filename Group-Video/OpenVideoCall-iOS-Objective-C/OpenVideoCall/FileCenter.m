//
//  FileCenter.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2019/5/6.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "FileCenter.h"

@implementation FileCenter
+ (NSString *)logDirectory {
    NSString *directory = [[self documentDirectory] stringByAppendingPathComponent:@"AgoraLogs"];
    [self checkAndCreateDirectoryAtPath:directory];
    return directory;
}

+ (NSString *)logFilePath {
    return [[self logDirectory] stringByAppendingPathComponent:@"agora-rtc.log"];
}

+ (NSString *)audioFilePath {
    return [[NSBundle mainBundle] pathForResource:@"Them" ofType:@"mp3"];
}

+ (NSString *)documentDirectory {
    return NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
}

+ (void)checkAndCreateDirectoryAtPath:(NSString *)path {
    BOOL isDirectory = NO;
    BOOL exists = [[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory];
    
    if (!exists || !isDirectory) {
        [[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    }
}
@end
