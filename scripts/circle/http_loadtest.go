// Copyright Circle Cardiovascular Imaging Inc.
//
// This software is licensed under the terms of the Circle Cardiovascular Imaging Inc.
// Software Usage Terms & Conditions. Please refer to:
//
//		 http://agreement.circlecvi.com

//+build ignore

package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"sync"
	"sync/atomic"
)

func get(url, username, password string) (*http.Response, error) {
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, fmt.Errorf("get: failed to create an http.Request object: %v", err)
	}

	req.SetBasicAuth(username, password)

	req.Header["Accept"] = []string{"application/json"}
	req.Header["Content-Type"] = []string{"application/json"}
	req.Close = true

	var client http.Client
	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf(
			"get: GET request returned error: %v", err)
	}
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("get: GET request failed with HTTP Status Code %d (%s)",
			resp.StatusCode,
			http.StatusText(resp.StatusCode))
	}

	return resp, nil
}

func main() {
	var wg sync.WaitGroup
	iterations := 20
	var successes int64
	url := "http://localhost:4299/api/v1/admin/enumeratelicenses"
	username := "admin"
	password := "password"

	flag.StringVar(&url, "url", url, "URL to GET")
	flag.StringVar(&username, "username", username, "username for auth")
	flag.StringVar(&password, "password", password, "password for auth")
	flag.IntVar(&iterations, "iterations", iterations, "Iterations to perform")

	flag.Parse()

	for i := 0; i < iterations; i++ {
		wg.Add(1)

		go func(iteration int) {
			_, err := get(url, username, password)
			if err != nil {
				log.Printf("iteration %d returned error: %v", iteration, err)
			} else {
				atomic.AddInt64(&successes, 1)
			}

			wg.Done()
		}(i)
	}

	wg.Wait()

	log.Printf("%d/%d success", successes, iterations)
}
