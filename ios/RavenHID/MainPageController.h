//
//  MainPageController.h
//  RavenHID
//
//  Created by lucas on 9/21/14.
//  Copyright (c) 2014 Lucas Morris. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLE.h"
#import "ScanTableViewController.h"

@interface MainPageController : UITableViewController <BLEDelegate, ScanTableViewDelegate, MFMailComposeViewControllerDelegate> {
    BLE *bleConnection;
}

@property (nonatomic, strong) IBOutlet UIBarButtonItem *ConnectButton;
@property (nonatomic, strong) NSMutableArray *GatheredCards;
@property (nonatomic, strong) NSMutableArray *GatheredBits;

@end
