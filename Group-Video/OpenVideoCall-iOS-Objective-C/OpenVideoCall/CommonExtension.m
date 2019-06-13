//
//  CommonExtension.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/4.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "CommonExtension.h"

@implementation UIColor (AGORA)
+ (UIColor *)AGTextGray {
    return [UIColor colorWithRed:102.0 / 255.0 green:102.0 / 255.0 blue:102.0 / 255.0 alpha:1];
}

+ (UIColor *)AGGray {
    return [UIColor colorWithRed:204.0 / 255.0 green:204.0 / 255.0 blue:204.0 / 255.0 alpha:1];
}

+ (UIColor *)AGBlue {
    return [UIColor colorWithRed:0 green:106.0 / 255.0 blue:216.0 / 255.0 alpha:1];
}

+ (UIColor *)AGMessageInfoColor {
    return [UIColor colorWithRed:0 green:0 blue:1 alpha:0.5];;
}
+ (UIColor *)AGMessageErrorColor {
    return [UIColor colorWithRed:1 green:0.5 blue:0 alpha:0.5];
}
@end
