//
//  MediaCharater.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/4.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "MediaCharater.h"

@implementation MediaCharater
+ (NSString *)updateToLegalMediaStringFromString:(NSString *)string {
    NSCharacterSet *legalMediaCharacterSet = [NSCharacterSet characterSetWithCharactersInString:@"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&()+,-:;<=.>?@[]^_`{|}~"];
    NSArray *separatedArray = [string componentsSeparatedByCharactersInSet:legalMediaCharacterSet.invertedSet];
    NSString *legalString = [separatedArray componentsJoinedByString:@""];
    return legalString;
}
@end
