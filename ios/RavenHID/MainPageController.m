//
//  MainPageControllerTableViewController.m
//  RavenHID
//
//  Created by lucas on 9/21/14.
//  Copyright (c) 2014 Lucas Morris. All rights reserved.
//

#import <AudioToolbox/AudioToolbox.h>
#import <MessageUI/MessageUI.h>
#import "MainPageController.h"
#import "MainTableCell.h"
#import "ScanTableViewController.h"


@interface MainPageController ()

@end

@implementation MainPageController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _GatheredCards = [[NSMutableArray alloc] init];
    _GatheredBits = [[NSMutableArray alloc] init];
    
    bleConnection = [[BLE alloc] init];
    [bleConnection controlSetup];
    bleConnection.delegate = self;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Ta  view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return _GatheredCards.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"MainTableCell";
    MainTableCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    // Configure the cell...
    long row = [indexPath row];
    cell.TitleLabel.text = _GatheredCards[row];
    
    NSString *bits = _GatheredBits[row];
    long bitlen = [bits length];
    
    cell.SizeLabel.text = [NSMutableString stringWithFormat:@"%ld bits", bitlen];

    return cell;
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if([[segue identifier] isEqualToString:@"ShowScan"]) {
        ScanTableViewController *scanviewcontroller = [segue destinationViewController];
        scanviewcontroller.delegate = self;
        scanviewcontroller.bleConnection = bleConnection;
    }
}

-(void)ScanTableViewControllerDismissed:(NSInteger *)peripheralIndex {
    NSLog(@"ScanTableViewControllerDismissed");
    
    if(bleConnection.peripherals.count > 0) {
        [bleConnection connectPeripheral:[bleConnection.peripherals objectAtIndex:*peripheralIndex]];
    }
}

-(void) bleDidReceiveData:(unsigned char *)data length:(int)length
{
    NSData *d = [NSData dataWithBytes:data length:length];
    NSString *s = [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
    s = [s stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    s = [s stringByReplacingOccurrencesOfString:@"'" withString:@""];
    NSLog(@"Got '%@' from device", s);
    
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"([01]{1,128}),([A-F0-9]{1,32})" options:0 error:NULL];
    NSTextCheckingResult *match = [regex firstMatchInString:s options:0 range:NSMakeRange(0, [s length])];
    
    if(match != nil) {
        NSString *bits = [s substringWithRange:[match rangeAtIndex:1]];
        [_GatheredCards addObject:[s substringWithRange:[match rangeAtIndex:2]]];
        [_GatheredBits addObject:bits];
    
        AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
        
        [self.tableView reloadData];
    }
}

-(void) bleDidConnect
{
    UIAlertView *alert = [[UIAlertView alloc]initWithTitle: @"Success!"
                                                   message: [NSMutableString stringWithFormat:@"Connected to %@", bleConnection.activePeripheral.name]
                                                  delegate: self
                                         cancelButtonTitle:@"OK"
                                         otherButtonTitles:nil,nil];
    
    [alert show];
    NSLog(@"bleDidConnect");
}

- (void) bleDidDisconnect
{
    UIAlertView *alert = [[UIAlertView alloc]initWithTitle: @"Notice"
                                                   message: @"BLE Device Disconnected"
                                                  delegate: self
                                         cancelButtonTitle:@"OK"
                                         otherButtonTitles:nil,nil];
    
    
    [alert show];
}

- (IBAction)SendCards:(id)sender {
    if ([MFMailComposeViewController canSendMail])
    {
        MFMailComposeViewController *mailer = [[MFMailComposeViewController alloc] init];
        
        mailer.mailComposeDelegate = self;
        [mailer setSubject:@"Information from RavenHID"];
        
        NSMutableString *emailBody = [NSMutableString stringWithString:@"The following information was gathered:\n\n\n"];
        [emailBody appendString:@"LENGTH,BITS,HEX\n"];
        
        for(int i = 0; i < _GatheredCards.count; i++) {
            long bitlen = [_GatheredBits[i] length];

            [emailBody appendFormat:@"%ld,%@,%@\n", bitlen, _GatheredBits[i], _GatheredCards[i]];
        }
        [mailer setMessageBody:emailBody isHTML:NO];
        
        [self presentViewController:mailer animated:YES completion:nil];
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Failure"
                                                        message:@"Your device doesn't support the sending email!"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
    }
}

-(void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error {
    switch (result)
    {
        case MFMailComposeResultCancelled:
            NSLog(@"Mail cancelled.");
            break;
        case MFMailComposeResultSaved:
            NSLog(@"Mail saved.");
            break;
        case MFMailComposeResultSent:
            NSLog(@"Mail send.");
            break;
        case MFMailComposeResultFailed:
            NSLog(@"Mail failed.");
            break;
        default:
            NSLog(@"Mail not sent.");
            break;
    }
    
    [controller dismissViewControllerAnimated:YES completion:nil];
}
/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
